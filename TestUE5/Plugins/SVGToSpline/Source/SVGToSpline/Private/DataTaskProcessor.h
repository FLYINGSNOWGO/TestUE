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
	class FPath;
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
	FTSTicker::FDelegateHandle TickerHandle;
private:
	bool Tick_GameThread(float DeltaTime);
public:
	FDataTaskProcessor(const FNotifyCompleteCmd& InNotifyProcessPathsComplete，const FNotifyUpdateMotionPoint& InNotifyUpdateMotionPoint);
	~FDataTaskProcessor();
	// Begin FRunnable [11/7/2022 CarlZhou]
	virtual uint32 Run() override;
	// End FRunnable [11/7/2022 CarlZhou]
	void Start();
	bool AddTask(TObjectPtr<USVGToSpline> InDataObj);

	void Tick_DataTaskProcessorThread();

};
