// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerGameInstanceDelegate.h"
#include "Common/Runtime/SqliteGameInstance.h"
#include "Log.h"

FServerGameInstanceDelegator::FServerGameInstanceDelegator()
{
	FScopeLog ServerGameInstanceDelegatorLog(*FString::Printf(TEXT("FServerGameInstanceDelegator::FServerGameInstanceDelegator,Address:%p"), this));
}

FServerGameInstanceDelegator::~FServerGameInstanceDelegator()
{
	FScopeLog ServerGameInstanceDelegatorLog(*FString::Printf(TEXT("FServerGameInstanceDelegator::~FServerGameInstanceDelegator,Address:%p"), this));
}

void FServerGameInstanceDelegator::Init(USqliteGameInstance *GameInstance)
{
	FScopeLog InitLog(TEXT("FServerGameInstanceDelegator::Init"), ELoggingOptions::LO_Console);
}
