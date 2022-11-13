// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Containers/Queue.h"
#include "Containers/Ticker.h"
#include "HAL/Runnable.h"
#include "CommonType.h"

DECLARE_LOG_CATEGORY_EXTERN(LogDataTaskProcessor, Log, All);

class FRunnableThread;
class USVGToSpline;

namespace DataTaskProcessorHelper
{
	class FPathsTask;
};

/**
 * 
 */
class FDataTaskProcessor : public FRunnable
{
private:
	TQueue<TSharedPtr<DataTaskProcessorHelper::FPathsTask>> Tasks;
private:
	/** The runnable thread */
	FRunnableThread* Thread;
	TAtomic<bool> RunThread;
	TSharedPtr<DataTaskProcessorHelper::FPathsTask> CurTask;
	FDelegateHandle CompleteCurTaskHandle;
	FTSTicker::FDelegateHandle TickerHandle;
private:
	void CompleteCurTask(const EMotionStatus InMotionStatus, const FVector2D& Inpoint);
	bool Tick_GameThread(float DeltaTime);

public:
	FDataTaskProcessor(const FNotifyCompleteCmd& InNotifyProcessPathsComplete,const FNotifyUpdateMotionPoint& InNotifyUpdateMotionPoint);
	~FDataTaskProcessor();
	// Begin FRunnable [11/7/2022 CarlZhou]
	virtual uint32 Run() override;
	// End FRunnable [11/7/2022 CarlZhou]
	void Start();
	bool AddTask(TObjectPtr<USVGToSpline> InDataObj);

	void Tick_DataTaskProcessorThread();

};
