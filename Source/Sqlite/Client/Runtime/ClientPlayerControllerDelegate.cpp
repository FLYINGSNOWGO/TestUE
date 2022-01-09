// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientPlayerControllerDelegate.h"
#include "Log.h"
#include "Async/Async.h"
#include "Common/Runtime/SqlitPlayerController.h"
#include "Kismet/GameplayStatics.h"

DECLARE_CYCLE_STAT(TEXT("  Check Client Network"), STAT_CheckClientNetwork, STATGROUP_SqlitPlayerController);


DECLARE_DWORD_COUNTER_STAT_EXTERN(TEXT("Test Curve"), STAT_TestCurve, STATGROUP_SqlitPlayerController, );
DECLARE_FLOAT_COUNTER_STAT_EXTERN(TEXT("TestFloat"), STAT_TestFloat, STATGROUP_SqlitPlayerController, );
DEFINE_STAT(STAT_TestCurve);
DEFINE_STAT(STAT_TestFloat);

FClientPlayerControllerDelegator::FClientPlayerControllerDelegator()
{
	FScopeLog ClientPlayerControllerDelegator(*FString::Printf(TEXT("FClientPlayerControllerDelegator::FClientPlayerControllerDelegator,Address:%p"), this));
	NetworkErroTimes = 0.f;
}

FClientPlayerControllerDelegator::~FClientPlayerControllerDelegator()
{
	FScopeLog ClientPlayerControllerDelegator(*FString::Printf(TEXT("FClientPlayerControllerDelegator::~FClientPlayerControllerDelegator,Address:%p"), this));
}

void FClientPlayerControllerDelegator::CheckClientNetwork(float DeltaTime)
{
	SCOPE_CYCLE_COUNTER(STAT_CheckClientNetwork);
	const double CurTime = FPlatformTime::Seconds();
	float PhysDeletaTime = CurTime - PreviousTime;
	PreviousTime = CurTime;
	if (PhysDeletaTime > 1000.f )
		return;
	//ULog::Info(*FString::Printf(TEXT("PhysDeletaTime:%f,DeltaTime:%f,CurTime:%f;PreviousTime:%f;networkErroTimes:%f,HeartBeatThresholdValue:%d"), PhysDeletaTime, DeltaTime, CurTime, PreviousTime, NetworkErroTimes, HeartBeatThresholdValue), ELoggingOptions::LO_Both, true, 0.f);
	NetworkErroTimes = NetworkErroTimes + PhysDeletaTime;

	SET_FLOAT_STAT(STAT_TestFloat, NetworkErroTimes);

	SET_DWORD_STAT(STAT_TestCurve, FMath::CeilToInt(NetworkErroTimes));
	if (NetworkErroTimes >= HeartBeatThresholdValue - 1)
	{
		//SqlitPlayerController->PrimaryActorTick.bCanEverTick = false;不起作用
		//SqlitPlayerController->PrimaryActorTick.SetTickFunctionEnable(false);
	}
		
	// 切到非GameThread执行
	AsyncTask(ENamedThreads::AnyThread, [=]()
	{
		//ULog::Info(*FString::Printf(TEXT("IP: ping success. Network appear anomaly, current HeartBeat times = %d, if more than %d times, think the client [Network Anomaly]. "), FMath::CeilToInt(NetworkErroTimes), HeartBeatThresholdValue), ELoggingOptions::LO_Both, true, 0.f);

	});
}

void FClientPlayerControllerDelegator::ConnectDSImplementation()
{
	FString with_ds_grpc = TEXT("true");
	FString IP = TEXT("10.12.32.129");
	FString Port = TEXT("7777");
	FString UserId = TEXT("1234567890");
	FString Platform = UGameplayStatics::GetPlatformName();

	TArray<FStringFormatArg> FormatArray;
	FormatArray.Add(FStringFormatArg(IP));
	FormatArray.Add(FStringFormatArg(Port));
	FormatArray.Add(FStringFormatArg(UserId));
	FormatArray.Add(FStringFormatArg(Platform));

	FString URL = FString::Format(TEXT("{0}:{1}?UserId={2}?Platform={3}"), FormatArray);

	ULog::Info(*FString::Printf(TEXT("==>URL:%s"), *URL), ELoggingOptions::LO_Both);

	SqlitPlayerController->ClientTravel(*URL, TRAVEL_Absolute);
}
