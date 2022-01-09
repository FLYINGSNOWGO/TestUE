// Fill out your copyright notice in the Description page of Project Settings.


#include "TestEventThread.h"
#include "Log.h"

FTestEventThread* FTestEventThread::TestEventThreadInstance = nullptr;

FTestEventThread::FTestEventThread()
{
	FScopeLog Info(FString::Printf(TEXT("TestEventThread::FTestEventThread,Add:%p"), this));
	//ThreadResumedEvent = FPlatformProcess::GetSynchEventFromPool();
	ThreadSuspendedEvent = FPlatformProcess::GetSynchEventFromPool();
	Thread = FRunnableThread::Create(this, TEXT("TestEventThread"));
}

FTestEventThread::~FTestEventThread()
{
	FScopeLog Info(FString::Printf(TEXT("TestEventThread::~FTestEventThread,Add:%p"), this));
	/*FPlatformProcess::ReturnSynchEventToPool(ThreadResumedEvent);
	ThreadResumedEvent = nullptr;*/
	FPlatformProcess::ReturnSynchEventToPool(ThreadSuspendedEvent);
	ThreadSuspendedEvent = nullptr;

	if (Thread != nullptr)
	{
		Thread->Kill(true);
		delete Thread;
	}
}

bool FTestEventThread::Init()
{
	FScopeLog Info(FString::Printf(TEXT("TestEventThread::Init,Add:%p"), this));
	return true;
}

uint32 FTestEventThread::Run()
{
	FScopeLog Info(FString::Printf(TEXT("FTestEventThread::Run,Add:%p"), this));
	double LastTime = FPlatformTime::Seconds();
	while (bDone)
	{
		if (bSuspend)
		{
			if (FPlatformTime::Seconds() - LastTime > 5.f)
			{
				bSuspend = false;
				FTestEventThreadHelper::Get()->UpdateLastTime();
				ThreadSuspendedEvent->Trigger();
				ULog::Info(FString::Printf(TEXT("TestEventThread::Run^^^Activity,Add:%p"), this), ELoggingOptions::LO_Both, true, 0.1f);
			}
			else
			{
				ULog::Info(FString::Printf(TEXT("TestEventThread::Run***Wait Activity,Add:%p"), this), ELoggingOptions::LO_Both, true, 0.1f);
			}
		}
		else
		{
			LastTime = FPlatformTime::Seconds();
			ULog::Info(FString::Printf(TEXT("TestEventThread::Run,Add:%p"), this), ELoggingOptions::LO_Both, true, 0.1f);
		}
		//UE_LOG(LogTemp, Display, TEXT("TestEventThread::Run,Add:%p"), this);

		FPlatformProcess::Sleep(0.5f);
	}
	return 0;
}

void FTestEventThread::Exit()
{
	FScopeLog Info(FString::Printf(TEXT("TestEventThread::Exit,Add:%p"), this));
}

void FTestEventThread::Stop()
{
	FScopeLog Info(FString::Printf(TEXT("TestEventThread::Stop,Add:%p"), this));
	bDone = false;
}

void FTestEventThread::CustomSuspendThread()
{
	FScopeLog Info(FString::Printf(TEXT("TestEventThread::CustomSuspendThread,Add:%p"), this));
	bSuspend = true;
	ThreadSuspendedEvent->Wait();
}

FThreadSafeBool FTestEventThread::GetSuspend()
{
	return bSuspend;
}

void FTestEventThread::SuspendThread()
{
	Thread->Suspend(true);
}

void FTestEventThread::ResumedThread()
{
	Thread->Suspend(false);
}

void FTestEventThread::ExitThread()
{
	delete TestEventThreadInstance;
	TestEventThreadInstance = nullptr;
}

FTestEventThreadHelper* FTestEventThreadHelper::TestEventThreadHelperInstance = nullptr;

FTestEventThreadHelper::FTestEventThreadHelper()
{
	Thread = FRunnableThread::Create(this, TEXT("FTestEventThreadHelper"));
}

FTestEventThreadHelper::~FTestEventThreadHelper()
{
	if (Thread != nullptr)
	{
		Thread->Kill(true);
		delete Thread;
		Thread = nullptr;
	}
}

bool FTestEventThreadHelper::Init()
{
	ULog::Info(TEXT("FTestEventThreadHelper::Init"));
	FTestEventThread::Get();
	return true;
}

uint32 FTestEventThreadHelper::Run()
{
	LastTime = FPlatformTime::Seconds();
	while (true)
	{
		if (FPlatformTime::Seconds() - LastTime > 5.f)
		{
			ULog::Info(FString::Printf(TEXT("FTestEventThreadHelper::Run!!!!!!,LastTime:%f,CurTime:%f,Interval:%f"), LastTime, FPlatformTime::Seconds(), FPlatformTime::Seconds() - LastTime));
			CustomSuspendThread();
		}
		else
		{
			ULog::Info(FString::Printf(TEXT("FTestEventThreadHelper::Run===>LastTime:%f,CurTime:%f,Interval:%f"), LastTime, FPlatformTime::Seconds(), FPlatformTime::Seconds() - LastTime));
			
		}
		FPlatformProcess::Sleep(0.5f);
	}
	return 0;
}

void FTestEventThreadHelper::Exit()
{
	ULog::Info(TEXT("FTestEventThreadHelper::Exit"));
}

void FTestEventThreadHelper::CustomSuspendThread()
{
	FTestEventThread::Get()->CustomSuspendThread();
}

void FTestEventThreadHelper::UpdateLastTime()
{
	LastTime = FPlatformTime::Seconds();
}
