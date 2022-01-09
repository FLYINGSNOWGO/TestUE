// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientCharacterDelegate.h"
#include "Log.h"

FClientCharacterDelegator::FClientCharacterDelegator()
{
	FScopeLog ClientCharacterDelegatorLog(*FString::Printf(TEXT("FClientCharacterDelegator::FClientCharacterDelegator,Address:%p"), this));
}

FClientCharacterDelegator::~FClientCharacterDelegator()
{
	FScopeLog ClientCharacterDelegatorLog(*FString::Printf(TEXT("FClientCharacterDelegator::~FClientCharacterDelegator,Address:%p"), this));
}
