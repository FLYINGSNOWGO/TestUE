// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Containers/Queue.h"
#include "Containers/Ticker.h"
#include "HAL/Runnable.h"

class FRunnableThread;
class USVGToSpline;

/**
 * 
 */
class FDataTaskProcessor : public FRunnable
{
private:
	TQueue<TObjectPtr<USVGToSpline>> Tasks;
private:
	/** The runnable thread */
	FRunnableThread* Thread;
	TAtomic<bool> RunThread;
	FTSTicker::FDelegateHandle TickerHandle;
private:
	bool Tick_GameThread(float DeltaTime);
public:
	FDataTaskProcessor();
	~FDataTaskProcessor();
	// Begin FRunnable [11/7/2022 CarlZhou]
	virtual uint32 Run() override;
	// End FRunnable [11/7/2022 CarlZhou]
	void Start();
	bool AddTask(TObjectPtr<USVGToSpline> Task);

	void Tick_DataTaskProcessorThread();

};
