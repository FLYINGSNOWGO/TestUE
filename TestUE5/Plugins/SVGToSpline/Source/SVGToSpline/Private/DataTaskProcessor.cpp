// Fill out your copyright notice in the Description page of Project Settings.


#include "DataTaskProcessor.h"
#include "DataTaskProcessor.h"
#include "DataTaskProcessor.h"
#include "HAL/RunnableThread.h"
#include "SVGToSplineActor.h"
#include "Components/TimelineComponent.h"

DEFINE_LOG_CATEGORY(LogDataTaskProcessor);

namespace DataTaskProcessorHelper
{
	class FPathSegment : public FGCObject
	{
	private:
		EMotionType MotionType;
		TArray<FVector2D> Datas;
		FVector2D StartPoint = FVector2D::ZeroVector;;
		FTimeline TimeLine;
		FVector2D NewPoint = FVector2D::ZeroVector;
		FVector2D LastPoint = FVector2D::ZeroVector;
		FVector2D Offset = FVector2D::ZeroVector;
		static UCurveFloat* TimeLineCurve;
		float ZoomFactor = 1.f;

		void OnTimeLineCallBack(float CurveValue);
		void OnTimeLineFinishedCallBack();

	public:
		FPathSegment(const FMotionData& InMotionData, const FVector2D& InOffset, const float InTimeLength, const float InZoomFactor);
		// Begin [11/9/2022 CarlZhou]
		virtual void AddReferencedObjects( FReferenceCollector& Collector ) override;
		virtual FString GetReferencerName() const override;
		// End [11/9/2022 CarlZhou]
		// 路径完成通知 [11/7/2022 CarlZhou]
		FNotifyCompleteCmd NotifyProcessPathSegmentComplete;
		static FNotifyUpdateMotionPoint NotifyUpdateMotionPoint;
		void SetStartPoint(const FVector2D& Inpoint);
		void SetLastPoint(const FVector2D& Inpoint);
		void DoWork();
		bool Tick_GameThread(float DeltaTime);
	};

	FNotifyUpdateMotionPoint FPathSegment::NotifyUpdateMotionPoint;
	UCurveFloat* FPathSegment::TimeLineCurve = LoadObject<UCurveFloat>(nullptr,TEXT("CurveFloat'/SVGToSpline/FloatCurve.FloatCurve'"));

	void FPathSegment::OnTimeLineCallBack(float CurveValue)
	{
		NewPoint = FVector2D::ZeroVector;
		switch (MotionType)
		{
			case EMotionType::MoveTo:
			case EMotionType::LineTo:
			{
				NewPoint = FMath::Lerp(StartPoint, Offset + Datas[0] * ZoomFactor,CurveValue);
				AsyncTask(ENamedThreads::GameThread,[=](){
					NotifyUpdateMotionPoint.ExecuteIfBound(MotionType, LastPoint, NewPoint);
				});
			}
			break;
			default:
			{
				UE_LOG(LogDataTaskProcessor, Warning, TEXT("OnTimeLineCallBack: MotionType:%s No Process"), MotionTypeToString(MotionType));
			}
		}
		SetLastPoint(NewPoint);
		UE_LOG(LogDataTaskProcessor, Log, TEXT("OnTimeLineCallBack: MotionType:%s;NewPoint:%s")
			, MotionTypeToString(MotionType)
			, *NewPoint.ToString());
	}

	void FPathSegment::OnTimeLineFinishedCallBack()
	{
		UE_LOG(LogDataTaskProcessor, Log, TEXT("OnTimeLineFinishedCallBack"));
		NotifyProcessPathSegmentComplete.Broadcast(NewPoint== Offset + Datas[Datas.Num() -1] * ZoomFactor ?
		EMotionStatus::OK : EMotionStatus::PathSegmentError
		, NewPoint);
	}

	FPathSegment::FPathSegment(const FMotionData& InMotionData, const FVector2D& InOffset, const float InTimeLength, const float InZoomFactor)
	{
		MotionType = InMotionData.MotionType;
		ZoomFactor = InZoomFactor;
		Offset = InOffset;
		Datas.Empty();
		Datas.Append(InMotionData.Datas);

		TimeLine.SetPlayRate(1.f / InTimeLength);

		FOnTimelineFloatStatic TimeLineCallBack;
		TimeLineCallBack.BindRaw(this, &FPathSegment::OnTimeLineCallBack);
		TimeLine.AddInterpFloat(TimeLineCurve, TimeLineCallBack);

		FOnTimelineEventStatic TimeLineFinishedCallBack;
		TimeLineFinishedCallBack.BindRaw(this, &FPathSegment::OnTimeLineFinishedCallBack);
		TimeLine.SetTimelineFinishedFunc(TimeLineFinishedCallBack);
	}

	void FPathSegment::AddReferencedObjects(FReferenceCollector& Collector)
	{
		Collector.AddReferencedObject(TimeLineCurve);
	}

	FString FPathSegment::GetReferencerName() const
	{
		return TEXT("FPathSegment");
	}

	void FPathSegment::SetStartPoint(const FVector2D& Inpoint)
	{
		StartPoint = Inpoint;
	}
	
	void FPathSegment::SetLastPoint(const FVector2D& Inpoint)
	{
		LastPoint = Inpoint;
	}

	void FPathSegment::DoWork()
	{
		TimeLine.PlayFromStart();
	}

	bool FPathSegment::Tick_GameThread(float DeltaTime)
	{
		TimeLine.TickTimeline(DeltaTime);
		return true;
	}

	class FPath
 	{
	private:
		TArray<TSharedPtr<FPathSegment>> PathSegments;
		FVector2D CurPoint;
		int32 CurSegmentIndex = 0;
		TSharedPtr<FPathSegment> CurPathSegment;
		float PathTimeLength = 1.f;
 	public:
		FPath(const FMotionPath& InMotionPath, const FVector2D& InOffset, const float InZoomFactor);
 		// 路径完成通知 [11/7/2022 CarlZhou]
 		FNotifyCompleteCmd NotifyProcessPathComplete;
		void OnNotifyProcessPathSegmentComplete(const EMotionStatus InMotionStatus, const FVector2D& Inpoint);
		void DoWork();
		void SetCurPoint(const FVector2D& Inpoint);
		bool Tick_GameThread(float DeltaTime);
 	};

	void FPath::SetCurPoint(const FVector2D& Inpoint)
	{
		CurPoint = Inpoint;
	}

	bool FPath::Tick_GameThread(float DeltaTime)
	{
		if (CurPathSegment.IsValid())
		{
			CurPathSegment->Tick_GameThread(DeltaTime);
		}
		return true;
	}

	void FPath::OnNotifyProcessPathSegmentComplete(const EMotionStatus InMotionStatus, const FVector2D& Inpoint)
	{
		if (InMotionStatus != EMotionStatus::OK)
		{
			UE_LOG(LogDataTaskProcessor,Warning,TEXT("FPath::OnNotifyProcessPathSegmentComplete MotionStatus:%s;Inpoint:%s"), 
			MotionStatusToString(InMotionStatus),*Inpoint.ToString());
		}
		CurSegmentIndex++;
		CurPoint = Inpoint;
		if (!PathSegments.IsValidIndex(CurSegmentIndex))
		{
			NotifyProcessPathComplete.Broadcast(InMotionStatus, Inpoint);
			return;
		}
		DoWork();
	}

	void FPath::DoWork()
	{
		if (!PathSegments.IsValidIndex(CurSegmentIndex))
		{
			NotifyProcessPathComplete.Broadcast(EMotionStatus::PathError, FVector2D::ZeroVector);
			UE_LOG(LogDataTaskProcessor, Error, TEXT("PathSegments Is Empty!"));
			return;
		}
		CurPathSegment = PathSegments[CurSegmentIndex];
		check(CurPathSegment.IsValid());
		CurPathSegment->SetStartPoint(CurPoint);
		CurPathSegment->SetLastPoint(CurPoint);
		CurPathSegment->DoWork();
	}

	FPath::FPath(const FMotionPath& InMotionPath, const FVector2D& InOffset, const float InZoomFactor)
	{
		int32 MotionDataNum = InMotionPath.MotionDatas.Num();
		for (const auto&MotionData : InMotionPath.MotionDatas )
		{
			TSharedPtr<FPathSegment> PathSegment = MakeShared<FPathSegment>(MotionData, InOffset, PathTimeLength / MotionDataNum, InZoomFactor);
			PathSegment->NotifyProcessPathSegmentComplete.AddRaw(this, &FPath::OnNotifyProcessPathSegmentComplete);
			PathSegments.Add(PathSegment);
		}
	}
	class FPathsTask
	{
	private:
		TArray<TSharedPtr<FPath>> Paths;
		FVector2D CurPoint;
		int32 CurPathIndex = 0;
		TSharedPtr<FPath> CurPath;
	public:
		// 完成所有路径通知[11/7/2022 CarlZhou]
		static FNotifyCompleteCmd NotifyProcessPathsComplete;
	public:
		FPathsTask(const FVector2D& InPoint, const TArray<FMotionPath>& MotionPaths,const float InZoomFactor = 1.f);
		void SetCurPoint(const FVector2D& InPoint);
		void OnNotifyProcessPathComplete(const EMotionStatus InMotionStatus, const FVector2D& Inpoint);
		void DoWork();
		bool Tick_GameThread(float DeltaTime);
	};

	FNotifyCompleteCmd FPathsTask::NotifyProcessPathsComplete;

	FPathsTask::FPathsTask(const FVector2D& InPoint, const TArray<FMotionPath>& InMotionPaths, const float InZoomFactor/* = 1.f*/)
	{
		CurPoint = InPoint;
		for (const auto& MotionPath : InMotionPaths)
		{
			TSharedPtr<FPath> Path = MakeShared<FPath>(MotionPath, InPoint, InZoomFactor);
			Path->NotifyProcessPathComplete.AddRaw(this,&FPathsTask::OnNotifyProcessPathComplete);
			Paths.Add(Path);
		}
	}

	void FPathsTask::SetCurPoint(const FVector2D& InPoint)
	{
		CurPoint = InPoint;
	}

	void FPathsTask::OnNotifyProcessPathComplete(const EMotionStatus InMotionStatus, const FVector2D& Inpoint)
	{
		UE_LOG(LogDataTaskProcessor, Log, TEXT("FPathsTask::OnNotifyProcessPathComplete MotionStatus:%s;Inpoint:%s"),
			MotionStatusToString(InMotionStatus), *Inpoint.ToString());
		CurPathIndex++;
		CurPoint = Inpoint;
		if (!Paths.IsValidIndex(CurPathIndex))
		{
			NotifyProcessPathsComplete.Broadcast(InMotionStatus, Inpoint);
			return;
		}
		DoWork();
	}

	void FPathsTask::DoWork()
	{
		if (!Paths.IsValidIndex(CurPathIndex))
		{
			NotifyProcessPathsComplete.Broadcast(EMotionStatus::PathsError, FVector2D::ZeroVector);
			UE_LOG(LogDataTaskProcessor, Error, TEXT("Paths Is Empty!"));
			return;
		}
		CurPath = Paths[CurPathIndex];
		check(CurPath.IsValid());
		CurPath->SetCurPoint(CurPoint);
		CurPath->DoWork();
	}

	bool FPathsTask::Tick_GameThread(float DeltaTime)
	{
		if (CurPath.IsValid())
		{
			CurPath->Tick_GameThread(DeltaTime);
		}
		return true;
	}

};

void FDataTaskProcessor::CompleteCurTask(const EMotionStatus InMotionStatus, const FVector2D& Inpoint)
{
	CurTask.Reset();
	UE_LOG(LogDataTaskProcessor, Log, TEXT("CurTask Complete! To Do Next Task"));
}

bool FDataTaskProcessor::Tick_GameThread(float DeltaTime)
{
	check(IsInGameThread());
	if (CurTask.IsValid())
	{
		CurTask->Tick_GameThread(DeltaTime);
	}
	return true;
}

FDataTaskProcessor::FDataTaskProcessor(const FNotifyCompleteCmd& InNotifyProcessPathsComplete,const FNotifyUpdateMotionPoint& InNotifyUpdateMotionPoint)
{
	RunThread = false;
	Thread = nullptr;
	DataTaskProcessorHelper::FPathsTask::NotifyProcessPathsComplete = InNotifyProcessPathsComplete;
	CompleteCurTaskHandle =  DataTaskProcessorHelper::FPathsTask::NotifyProcessPathsComplete.AddRaw(this,&FDataTaskProcessor::CompleteCurTask);
	DataTaskProcessorHelper::FPathSegment::NotifyUpdateMotionPoint = InNotifyUpdateMotionPoint;
}

FDataTaskProcessor::~FDataTaskProcessor()
{
	RunThread = false;
	FTSTicker::GetCoreTicker().RemoveTicker(TickerHandle);

	if (Thread)
	{
		delete Thread;
		Thread = nullptr;
	}
	CurTask.Reset();
	check(CompleteCurTaskHandle.IsValid());
	DataTaskProcessorHelper::FPathsTask::NotifyProcessPathsComplete.Remove(CompleteCurTaskHandle);
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
	TSharedPtr<DataTaskProcessorHelper::FPathsTask> PathsTask = MakeShared<DataTaskProcessorHelper::FPathsTask>(InDataObj->CurPoint,InDataObj->MotionPaths, InDataObj->ZoomFactor);
	return Tasks.Enqueue(PathsTask);
}

void FDataTaskProcessor::Tick_DataTaskProcessorThread()
{
	while (RunThread)
	{
		if (!CurTask.IsValid())
		{
			if (Tasks.Dequeue(CurTask) && CurTask.IsValid())
			{
				CurTask->DoWork();
			}
		}
		//FPlatformProcess::Sleep(0.1);
	}
}
