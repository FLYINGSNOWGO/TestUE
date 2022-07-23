// Fill out your copyright notice in the Description page of Project Settings.


#include "NCCharacter.h"
#include "Common/NCLogChannels.h"
#include "Common/Factory/DelegatorFactory.h"

// Sets default values
ANCCharacter::ANCCharacter()
{
	UE_LOG(LogNC, Log, TEXT("Enter ANCPlayerController::ANCCharacter[%s]"), *GetClientServerContextString(this));
	if (!HasAnyFlags(RF_ClassDefaultObject) && GetGameInstance() && GetGameInstance()->IsDedicatedServerInstance())
	{
		UE_LOG(LogNC, Log, TEXT("ANCCharacter::ANCCharacter->IsDedicatedServerInstance: true"));
	}
	else
	{
		UE_LOG(LogNC, Log, TEXT("ANCCharacter::ANCCharacter->IsDedicatedServerInstance: false"));
	}

	if (!HasAnyFlags(RF_ClassDefaultObject) && GetGameInstance())
	{
		CharacterDelegator = FDelegatorFactory::CreateCharacterDelegator(FDelegatorFactory::GetType(GetGameInstance()->IsDedicatedServerInstance()));
	}
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	UE_LOG(LogNC, Log, TEXT("Exit ANCPlayerController::ANCCharacter[%s]"), *GetClientServerContextString(this));
}

// Called when the game starts or when spawned
void ANCCharacter::BeginPlay()
{
	UE_LOG(LogNC, Log, TEXT("Enter ANCCharacter::BeginPlay[%s]"), *GetClientServerContextString(this));
	if (GetGameInstance() && GetGameInstance()->IsDedicatedServerInstance())
	{
		UE_LOG(LogNC, Log, TEXT("ANCCharacter::BeginPlay->IsDedicatedServerInstance: true"));
	}
	else
	{
		UE_LOG(LogNC, Log, TEXT("ANCCharacter::BeginPlay->IsDedicatedServerInstance: false"));
	}
	Super::BeginPlay();
	UE_LOG(LogNC, Log, TEXT("Exit ANCCharacter::BeginPlay[%s]"), *GetClientServerContextString(this));
}

// Called every frame
void ANCCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ANCCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

