// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientCharacterDelegator.h"
#include "Common/NCLogChannels.h"
#include "Kismet/GameplayStatics.h"

UClientCharacterDelegator::UClientCharacterDelegator(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/) : Super(ObjectInitializer)
{
	UE_LOG(LogClientNC, Log, TEXT("Enter UClientCharacterDelegator::UClientCharacterDelegator[%s]"), *GetClientServerContextString(this));

	UE_LOG(LogClientNC, Log, TEXT("Exit UClientCharacterDelegator::UClientCharacterDelegator[%s]"), *GetClientServerContextString(this));
}

void UClientCharacterDelegator::TestCharacterDelegator()
{
	UE_LOG(LogClientNC,Log,TEXT("Enter UClientCharacterDelegator::TestCharacterDelegator[%s]"),*GetClientServerContextString(this));

	UE_LOG(LogClientNC,Log,TEXT("Exit UClientCharacterDelegator::TestCharacterDelegator[%s]"),*GetClientServerContextString(this));
}
