// Fill out your copyright notice in the Description page of Project Settings.


#include "NCPlayerController.h"
#include "Common/NCLogChannels.h"
#include "Common/Factory/DelegatorFactory.h"
#include "Common/Delegate/PlayerControllerDelegator.h"


void ANCPlayerController::BeginPlay()
{
	UE_LOG(LogNC,Log,TEXT("Enter ANCPlayerController::BeginPlay[%s], this:%p"),*GetClientServerContextString(this),this);
	if (GetGameInstance()->IsDedicatedServerInstance())
	{
		UE_LOG(LogNC, Log, TEXT("ANCPlayerController::BeginPlay->IsDedicatedServerInstance: true"));
	}
	else
	{
		UE_LOG(LogNC, Log, TEXT("ANCPlayerController::BeginPlay->IsDedicatedServerInstance: false"));
	}
	Super::BeginPlay();
	PlayerControllerDelegator->BeginPlay();
	UE_LOG(LogNC,Log,TEXT("Exit ANCPlayerController::BeginPlay[%s], this:%p"),*GetClientServerContextString(this),this);
}

ANCPlayerController::ANCPlayerController(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	UE_LOG(LogNC,Log,TEXT("Enter ANCPlayerController::ANCPlayerController[%s], this:%p"),*GetClientServerContextString(this),this);
	if (!HasAnyFlags(RF_ClassDefaultObject) && GetGameInstance() && GetGameInstance()->IsDedicatedServerInstance())
	{
		UE_LOG(LogNC, Log, TEXT("ANCPlayerController::ANCPlayerController->IsDedicatedServerInstance: true"));
	}
	else
	{
		UE_LOG(LogNC, Log, TEXT("ANCPlayerController::ANCPlayerController->IsDedicatedServerInstance: false"));
	}
	if (!HasAnyFlags(RF_ClassDefaultObject) && GetGameInstance())
	{
		PlayerControllerDelegator = FDelegatorFactory::CreatePlayerControllerDelegator(FDelegatorFactory::GetType(GetGameInstance()->IsDedicatedServerInstance()));
		PlayerControllerDelegator->Init(this);
	}
	UE_LOG(LogNC, Log, TEXT("Exit ANCPlayerController::ANCPlayerController[%s], this:%p"), *GetClientServerContextString(this),this);
}
