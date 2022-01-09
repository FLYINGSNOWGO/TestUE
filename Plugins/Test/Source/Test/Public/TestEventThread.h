// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HAL/Runnable.h"

class TEST_API FTestEventThreadHelper : public FRunnable
{
private:
	/** Holds the thread object. */
	FRunnableThread* Thread;
	static FTestEventThreadHelper* TestEventThreadHelperInstance;
	double LastTime;
public:
	FTestEventThreadHelper();
	virtual ~FTestEventThreadHelper();
public:
	/** Returns the async loading thread singleton */
	static FTestEventThreadHelper* Get()
	{
		if (!TestEventThreadHelperInstance)
		{
			TestEventThreadHelperInstance = new FTestEventThreadHelper();
		}
		return TestEventThreadHelperInstance;
	}
public:
	// start FRunnable Interface [12/16/2021 CarlZhou]
	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Exit() override;
	// end FRunnable Interface [12/16/2021 CarlZhou]
	void CustomSuspendThread();
	void UpdateLastTime();
};

/**
 * 
 */
class TEST_API FTestEventThread : public FRunnable
{
private:
	FEvent* ThreadSuspendedEvent;
	//FEvent* ThreadResumedEvent;
	/** Holds the thread object. */
	FRunnableThread* Thread;
	static FTestEventThread* TestEventThreadInstance;
	FThreadSafeBool bSuspend = false;
	bool bDone = true;
public:
	FTestEventThread();
	virtual ~FTestEventThread();
public:
	/** Returns the async loading thread singleton */
	static FTestEventThread* Get()
	{
		if (!TestEventThreadInstance)
		{
			TestEventThreadInstance = new FTestEventThread();
		}
		return TestEventThreadInstance;
	}
public:
	// start FRunnable Interface [12/16/2021 CarlZhou]
	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Exit() override;
	virtual void Stop() override;
	// end FRunnable Interface [12/16/2021 CarlZhou]
	void CustomSuspendThread();
	FThreadSafeBool GetSuspend();
	// Suspends the task related thread [12/17/2021 CarlZhou]
	void SuspendThread();
	// Restore the task related threads [12/17/2021 CarlZhou]
	void ResumedThread();
	void ExitThread();
};
