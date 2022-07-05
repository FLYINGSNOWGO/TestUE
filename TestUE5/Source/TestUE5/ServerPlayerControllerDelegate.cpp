// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerPlayerControllerDelegate.h"
#include "TestLogChannels.h"

void UServerPlayerControllerDelegate::TestPlayerController()
{
	UE_LOG(LogServerNC,Log,TEXT("Enter UServerPlayerControllerDelegate::TestPlayerController[%s]"),*GetClientServerContextString(this));
	UE_LOG(LogServerNC,Log,TEXT("Exit UServerPlayerControllerDelegate::TestPlayerController[%s]"),*GetClientServerContextString(this));
}
