// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerPlayerControllerDelegator.h"
#include "Common/NCLogChannels.h"
#include "Kismet/GameplayStatics.h"

UServerPlayerControllerDelegator::UServerPlayerControllerDelegator(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/) : Super(ObjectInitializer)
{
	UE_LOG(LogServerNC, Log, TEXT("Enter UServerPlayerControllerDelegate::UServerPlayerControllerDelegator[%s], this:%p"), *GetClientServerContextString(this),this);
	UE_LOG(LogServerNC, Log, TEXT("Exit UServerPlayerControllerDelegate::UServerPlayerControllerDelegator[%s], this:%p"), *GetClientServerContextString(this),this);
}

void UServerPlayerControllerDelegator::TestPlayerController()
{
	UE_LOG(LogServerNC,Log,TEXT("Enter UServerPlayerControllerDelegate::TestPlayerController[%s], this:%p"),*GetClientServerContextString(this),this);
	UE_LOG(LogServerNC,Log,TEXT("Exit UServerPlayerControllerDelegate::TestPlayerController[%s], this:%p"),*GetClientServerContextString(this),this);
}
