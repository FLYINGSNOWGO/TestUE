// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientGameInstanceDelegator.h"
#include "Common/NCLogChannels.h"
#include "Kismet/GameplayStatics.h"

UClientGameInstanceDelegator::UClientGameInstanceDelegator(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/) : Super(ObjectInitializer)
{
	UE_LOG(LogServerNC, Log, TEXT("Enter UClientGameInstanceDelegator::UClientGameInstanceDelegator[%s]"), *GetClientServerContextString(this));
	UE_LOG(LogServerNC, Log, TEXT("Exit UClientGameInstanceDelegator::UClientGameInstanceDelegator[%s]"), *GetClientServerContextString(this));
}

void UClientGameInstanceDelegator::TestGameInstanceDelegator()
{
	UE_LOG(LogServerNC,Log,TEXT("Enter UClientGameInstanceDelegator::TestGameInstanceDelegator[%s]"),*GetClientServerContextString(this));
	UE_LOG(LogServerNC,Log,TEXT("Exit UClientGameInstanceDelegator::TestGameInstanceDelegator[%s]"),*GetClientServerContextString(this));
}
