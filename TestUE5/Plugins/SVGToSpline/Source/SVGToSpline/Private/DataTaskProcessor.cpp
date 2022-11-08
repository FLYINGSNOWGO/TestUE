// Fill out your copyright notice in the Description page of Project Settings.


#include "DataTaskProcessor.h"
#include "HAL/RunnableThread.h"
#include "CommonType.h"

namespace DataTaskProcessorHelper
{
	class FTaskPaths
	{
	private:
		TArray<FMotionPath> MotionPaths;
		FVector2D CurPoint;
		int32 CurPathIndex = 0;
	public:
		// 完成当前所有路径处理通知 [11/7/2022 CarlZhou]
		FNotifyCompleteCmd NotifyProcessPathsComplete;
		// 完成当前路径处理通知 [11/7/2022 CarlZhou]
		FNotifyCompleteCmd NotifyProcessPathComplete;
	public:
		FTaskPaths(const FVector2D& InPoint, const TArray<FMotionPath>& MotionPaths);
		void SetCurPoint(const FVector2D& InPoint);
		void OnNotifyProcessPathComplete(const EMotionStatus InMotionStatus, const FVector2D& Inpoint);
		void ProcessPath();
	};

	class FPath
	{
		
	};

	FTaskPaths::FTaskPaths(const FVector2D& InPoint, const TArray<FMotionPath>& InMotionPaths)
	{
		MotionPaths.Empty();
		MotionPaths.Append(InMotionPaths);
		CurPoint = InPoint;
	}

	void FTaskPaths::SetCurPoint(const FVector2D& InPoint)
	{
		CurPoint = InPoint;
	}

	void FTaskPaths::OnNotifyProcessPathComplete(const EMotionStatus InMotionStatus, const FVector2D& Inpoint)
	{
		CurPathIndex++;
		CurPoint = Inpoint;
		if (!MotionPaths.IsValidIndex(CurPathIndex))
		{
			NotifyProcessPathsComplete.ExecuteIfBound(InMotionStatus, Inpoint);
			return;
		}
		ProcessPath();
	}

	void FTaskPaths::ProcessPath()
	{
		
	}

};

bool FDataTaskProcessor::Tick_GameThread(float DeltaTime)
{
	check(IsInGameThread());
	return true;
}

FDataTaskProcessor::FDataTaskProcessor()
{
	RunThread = false;
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

bool FDataTaskProcessor::AddTask(TObjectPtr<USVGToSpline> Task)
{
	return Tasks.Enqueue(Task);
}

void FDataTaskProcessor::Tick_DataTaskProcessorThread()
{
	while (RunThread)
	{
		TObjectPtr<USVGToSpline> Task;
		if (Tasks.Enqueue(Task))
		{
			
		}
	}
}
