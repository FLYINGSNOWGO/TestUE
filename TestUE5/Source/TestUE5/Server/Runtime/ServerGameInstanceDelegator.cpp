// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerGameInstanceDelegator.h"
#include "Common/NCLogChannels.h"
#include "Kismet/GameplayStatics.h"

UServerGameInstanceDelegator::UServerGameInstanceDelegator(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/) : Super(ObjectInitializer)
{
	UE_LOG(LogServerNC, Log, TEXT("Enter UServerGameInstanceDelegator::UServerGameInstanceDelegator[%s]"), *GetClientServerContextString(this));
	UE_LOG(LogServerNC, Log, TEXT("Exit UServerGameInstanceDelegator::UServerGameInstanceDelegator[%s]"), *GetClientServerContextString(this));
}

void UServerGameInstanceDelegator::TestGameInstanceDelegator()
{
	UE_LOG(LogServerNC,Log,TEXT("Enter UServerGameInstanceDelegator::TestGameInstanceDelegator[%s]"),*GetClientServerContextString(this));
	UE_LOG(LogServerNC,Log,TEXT("Exit UServerGameInstanceDelegator::TestGameInstanceDelegator[%s]"),*GetClientServerContextString(this));
}
