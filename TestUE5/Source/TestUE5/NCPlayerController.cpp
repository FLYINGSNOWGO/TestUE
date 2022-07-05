// Fill out your copyright notice in the Description page of Project Settings.


#include "NCPlayerController.h"
#include "TestLogChannels.h"
#include "ServerPlayerControllerDelegate.h"
#include "ClientPlayerControllerDelegate.h"


void ANCPlayerController::BeginPlay()
{
	UE_LOG(LogNC,Log,TEXT("Enter ANCPlayerController::BeginPlay[%s]"),*GetClientServerContextString(this));
	Super::BeginPlay();
	PlayerControllerInterface->TestPlayerController();
	UE_LOG(LogNC,Log,TEXT("Exit ANCPlayerController::BeginPlay[%s]"),*GetClientServerContextString(this));
}

ANCPlayerController::ANCPlayerController(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	UE_LOG(LogNC,Log,TEXT("Enter ANCPlayerController::ANCPlayerController[%s]"),*GetClientServerContextString(this));
	UE_LOG(LogNC,Log,TEXT("Exit ANCPlayerController::ANCPlayerController[%s]"),*GetClientServerContextString(this));
	if (GetClientServerContextString(this) == TEXT("Server"))
	{
		PlayerControllerInterface = NewObject<UServerPlayerControllerDelegate>();
	}
	else if (GetClientServerContextString(this) == TEXT("Client"))
	{
		PlayerControllerInterface = NewObject<UClientPlayerControllerDelegate>();
	}
}
