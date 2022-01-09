// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerPlayerControllerDelegate.h"
#include "HAL/PlatformFilemanager.h"
#include "IPlatformFilePak.h"
#include "Common/Runtime/SqliteGameInstance.h"
#include "Log.h"

FServerPlayerControllerDelegator::~FServerPlayerControllerDelegator()
{
	FScopeLog ServerPlayerControllerDelegatorLog(*FString::Printf(TEXT("FServerPlayerControllerDelegator::~FServerPlayerControllerDelegator,Address:%p"), this));
}

FServerPlayerControllerDelegator::FServerPlayerControllerDelegator()
{
	FScopeLog ServerPlayerControllerDelegatorLog(*FString::Printf(TEXT("FServerPlayerControllerDelegator::FServerPlayerControllerDelegator,Address:%p"), this));
}
