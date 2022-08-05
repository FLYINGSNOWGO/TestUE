// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerCharacterDelegator.h"
#include "Common/NCLogChannels.h"
#include "Kismet/GameplayStatics.h"

UServerCharacterDelegator::UServerCharacterDelegator(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/):Super(ObjectInitializer)
{
	UE_LOG(LogServerNC, Log, TEXT("Enter UServerCharacterDelegator::UServerCharacterDelegator[%s], this:%p"), *GetClientServerContextString(this),this);
	UE_LOG(LogServerNC, Log, TEXT("Exit UServerCharacterDelegator::UServerCharacterDelegator[%s], this:%p"), *GetClientServerContextString(this),this);
}

void UServerCharacterDelegator::TestCharacterDelegator()
{
	UE_LOG(LogServerNC,Log,TEXT("Enter UServerCharacterDelegator::TestCharacterDelegator[%s], this:%p"),*GetClientServerContextString(this), this);
	UE_LOG(LogServerNC,Log,TEXT("Exit UServerCharacterDelegator::TestCharacterDelegator[%s], this:%p"),*GetClientServerContextString(this),this);
}
