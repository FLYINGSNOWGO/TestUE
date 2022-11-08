// Fill out your copyright notice in the Description page of Project Settings.


#include "DataTaskProcessor.h"
#include "DataTaskProcessor.h"
#include "DataTaskProcessor.h"
#include "HAL/RunnableThread.h"
#include "SVGToSplineActor.h"

DEFINE_LOG_CATEGORY(LogDataTaskProcessor);

namespace DataTaskProcessorHelper
{
	class FPathSegment
	{
	private:
		EMotionType MotionType;
		TArray<FVector2D> Datas;
		FVector2D CurPoint;
	public:
		FPathSegment(const FMotionData& InMotionData);
		// 路径完成通知 [11/7/2022 CarlZhou]
		FNotifyCompleteCmd NotifyProcessPathSegmentComplete;
		static FNotifyUpdateMotionPoint NotifyUpdateMotionPoint;
		void SetCurPoint(const FVector2D& Inpoint);
		void DoWork();
	};

	FPathSegment::FPathSegment(const FMotionData& InMotionData)
	{
		MotionType = InMotionData.MotionType;
		Datas.Empty();
		Datas.Append(InMotionData.Datas);
	}

	void FPathSegment::SetCurPoint(const FVector2D& Inpoint)
	{
		CurPoint = Inpoint;
	}
	
	void FPathSegment::DoWork()
	{
		
	}

	class FPath
 	{
	private:
		TArray<TSharedPtr<FPathSegment>> PathSegments;
		FVector2D CurPoint;
		int32 CurSegmentIndex = 0;
 	public:
		FPath(const FMotionPath& InMotionPath);
 		// 路径完成通知 [11/7/2022 CarlZhou]
 		FNotifyCompleteCmd NotifyProcessPathComplete;
		void OnNotifyProcessPathSegmentComplete(const EMotionStatus InMotionStatus, const FVector2D& Inpoint);
		void DoWork();
		void SetCurPoint(const FVector2D& Inpoint);
 	};

	void FPath::SetCurPoint(const FVector2D& Inpoint)
	{
		CurPoint = Inpoint;
	}

	void FPath::OnNotifyProcessPathSegmentComplete(const EMotionStatus InMotionStatus, const FVector2D& Inpoint)
	{
		CurSegmentIndex++;
		CurPoint = Inpoint;
		if (!PathSegments.IsValidIndex(CurSegmentIndex))
		{
			const bool bExeSuccess = NotifyProcessPathComplete.ExecuteIfBound(InMotionStatus, Inpoint);
			if (!bExeSuccess)
			{
				UE_LOG(LogDataTaskProcessor,Warning,TEXT("NotifyProcessPathComplete Not Bind"));
			}
			return;
		}
		DoWork();
	}

	void FPath::DoWork()
	{
		if (!PathSegments.IsValidIndex(CurSegmentIndex))
		{
			const bool bExeSuccess = NotifyProcessPathComplete.ExecuteIfBound(EMotionStatus::PathError, FVector2D::ZeroVector);
			if (!bExeSuccess)
			{
				UE_LOG(LogDataTaskProcessor,Error,TEXT("PathSegments Is Empty!"));
			}
			return;
		}
		TSharedPtr<FPathSegment> PathSegment = PathSegments[CurSegmentIndex];
		check(PathSegment.IsValid());
		PathSegment->SetCurPoint(CurPoint);
		PathSegment->DoWork();
	}

	FPath::FPath(const FMotionPath& InMotionPath)
	{
		for (const auto&MotionData : InMotionPath.MotionDatas )
		{
			TSharedPtr<FPathSegment> Path = MakeShared<FPathSegment>(MotionData);
			PathSegments.Add(Path);
		}
	}
	class FPathsTask
	{
	private:
		TArray<TSharedPtr<FPath>> Paths;
		FVector2D CurPoint;
		int32 CurPathIndex = 0;
	public:
		// 完成所有路径通知[11/7/2022 CarlZhou]
		static FNotifyCompleteCmd NotifyProcessPathsComplete;
	public:
		FPathsTask(const FVector2D& InPoint, const TArray<FMotionPath>& MotionPaths);
		void SetCurPoint(const FVector2D& InPoint);
		void OnNotifyProcessPathComplete(const EMotionStatus InMotionStatus, const FVector2D& Inpoint);
		void DoWork();
	};
	FPathsTask::FPathsTask(const FVector2D& InPoint, const TArray<FMotionPath>& InMotionPaths)
	{
		CurPoint = InPoint;
		for (const auto& MotionPath : InMotionPaths)
		{
			TSharedPtr<FPath> Path = MakeShared<FPath>(MotionPath);
			Path->NotifyProcessPathComplete.BindRaw(this,&FPathsTask::OnNotifyProcessPathComplete);
			Paths.Add(Path);
		}
	}

	void FPathsTask::SetCurPoint(const FVector2D& InPoint)
	{
		CurPoint = InPoint;
	}

	void FPathsTask::OnNotifyProcessPathComplete(const EMotionStatus InMotionStatus, const FVector2D& Inpoint)
	{
		CurPathIndex++;
		CurPoint = Inpoint;
		if (!Paths.IsValidIndex(CurPathIndex))
		{
			const bool bExeSuccess = NotifyProcessPathsComplete.ExecuteIfBound(InMotionStatus, Inpoint);
			if (!bExeSuccess)
			{
				UE_LOG(LogDataTaskProcessor,Warning,TEXT("NotifyProcessPathsComplete Not Bind"));
			}
			return;
		}
		DoWork();
	}

	void FPathsTask::DoWork()
	{
		if (!Paths.IsValidIndex(CurPathIndex))
		{
			const bool bExeSuccess = NotifyProcessPathsComplete.ExecuteIfBound(EMotionStatus::PathsError, FVector2D::ZeroVector);
			if (!bExeSuccess)
			{
				UE_LOG(LogDataTaskProcessor,Error,TEXT("Paths Is Empty!"));
			}
			return;
		}
		TSharedPtr<FPath> Path = Paths[CurPathIndex];
		check(Path.IsValid());
		Path->SetCurPoint(CurPoint);
		Path->DoWork();
	}

};

bool FDataTaskProcessor::Tick_GameThread(float DeltaTime)
{
	check(IsInGameThread());
	return true;
}

FDataTaskProcessor::FDataTaskProcessor(const FNotifyCompleteCmd& InNotifyProcessPathsComplete，const FNotifyUpdateMotionPoint& InNotifyUpdateMotionPoint)
{
	RunThread = false;
	Thread = nullptr;
	DataTaskProcessorHelper::FPathsTask::NotifyProcessPathsComplete = InNotifyProcessPathsComplete;
	DataTaskProcessorHelper::FPathSegment::NotifyUpdateMotionPoint = InNotifyUpdateMotionPoint;
}

FDataTaskProcessor::~FDataTaskProcessor()
{
	RunThread = false;

	if (Thread)
	{
		delete Thread;
		Thread = nullptr;
	}

	FTSTicker::GetCoreTicker().RemoveTicker(TickerHandle);
}

uint32 FDataTaskProcessor::Run()
{
	Tick_DataTaskProcessorThread();
	return 0;
}

void FDataTaskProcessor::Start()
{
	TickerHandle = FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateRaw(this, &FDataTaskProcessor::Tick_GameThread), 0);
	RunThread = true;
	Thread = FRunnableThread::Create(this, TEXT("DataTaskProcessor"), 0, TPri_Normal, FPlatformAffinity::GetPoolThreadMask());
}

bool FDataTaskProcessor::AddTask(TObjectPtr<USVGToSpline> InDataObj)
{
	TSharedPtr<DataTaskProcessorHelper::FPathsTask> PathsTask = MakeShared<DataTaskProcessorHelper::FPathsTask>(FVector2D::ZeroVector,InDataObj->MotionPaths);
	return Tasks.Enqueue(PathsTask);
}

void FDataTaskProcessor::Tick_DataTaskProcessorThread()
{
	while (RunThread)
	{
		TSharedPtr<DataTaskProcessorHelper::FPathsTask> Task;
		if (Tasks.Enqueue(Task))
		{
			
		}
	}
}
