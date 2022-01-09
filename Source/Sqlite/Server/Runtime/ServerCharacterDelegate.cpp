// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerCharacterDelegate.h"
#include "Log.h"

FServerCharacterDelegator::FServerCharacterDelegator()
{
	FScopeLog ServerCharacterDelegatorLog(*FString::Printf(TEXT("FServerCharacterDelegator::FServerCharacterDelegator,Address:%p"), this));
}

FServerCharacterDelegator::~FServerCharacterDelegator()
{
	FScopeLog ServerCharacterDelegatorLog(*FString::Printf(TEXT("FServerCharacterDelegator::~FServerCharacterDelegator,Address:%p"), this));
}