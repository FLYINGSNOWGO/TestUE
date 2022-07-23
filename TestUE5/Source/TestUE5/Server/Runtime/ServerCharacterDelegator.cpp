// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerCharacterDelegator.h"
#include "Common/NCLogChannels.h"
#include "Kismet/GameplayStatics.h"

UServerCharacterDelegator::UServerCharacterDelegator(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/):Super(ObjectInitializer)
{
	UE_LOG(LogServerNC, Log, TEXT("Enter UServerCharacterDelegator::UServerCharacterDelegator[%s]"), *GetClientServerContextString(this));
	UE_LOG(LogServerNC, Log, TEXT("Exit UServerCharacterDelegator::UServerCharacterDelegator[%s]"), *GetClientServerContextString(this));
}

void UServerCharacterDelegator::TestCharacterDelegator()
{
	UE_LOG(LogServerNC,Log,TEXT("Enter UServerCharacterDelegator::TestCharacterDelegator[%s]"),*GetClientServerContextString(this));
	UE_LOG(LogServerNC,Log,TEXT("Exit UServerCharacterDelegator::TestCharacterDelegator[%s]"),*GetClientServerContextString(this));
}
