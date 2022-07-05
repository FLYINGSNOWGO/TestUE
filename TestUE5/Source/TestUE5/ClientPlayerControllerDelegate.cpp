// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientPlayerControllerDelegate.h"
#include "TestLogChannels.h"


void UClientPlayerControllerDelegate::TestPlayerController()
{
	UE_LOG(LogClientNC,Log,TEXT("Enter UClientPlayerControllerDelegate::TestPlayerController[%s]"),*GetClientServerContextString(this));
	UE_LOG(LogClientNC,Log,TEXT("Exit UClientPlayerControllerDelegate::TestPlayerController[%s]"),*GetClientServerContextString(this));
}
