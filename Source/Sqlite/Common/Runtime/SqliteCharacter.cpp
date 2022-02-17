// Fill out your copyright notice in the Description page of Project Settings.


#include "SqliteCharacter.h"
#include "Log.h"
#include "Net/UnrealNetwork.h"
#include "Engine/NetConnection.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASqliteCharacter::ASqliteCharacter()
{
	FScopeLog SqliteCharacterLog(*FString::Printf(TEXT("ASqliteCharacter::ASqliteCharacter,Address:%p"), this));
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true; 
	bReplicates = true;
}

// Called when the game starts or when spawned
void ASqliteCharacter::BeginPlay()
{
	FScopeLog BeginPlayLog(*FString::Printf(TEXT("ASqliteCharacter::BeginPlay,Address:%p,Name:%s"), this,*GetName()));
	Super::BeginPlay();
	if (UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn() == this)
	{
		ULog::Info(*FString::Printf(TEXT("==>ASqliteCharacter::BeginPlay:Charater is self!Address:%p"), this));
	}
	if (GetRemoteRole() == ENetRole::ROLE_Authority)
	{
		ULog::Info(*FString::Printf(TEXT("==>ASqliteCharacter::BeginPlay:RemoteRole:ROLE_Authority,Address:%p"),this));
	}
	else if (GetRemoteRole() == ENetRole::ROLE_AutonomousProxy)
	{
		ULog::Info(*FString::Printf(TEXT("==>ASqliteCharacter::BeginPlay:RemoteRole:ROLE_AutonomousProxy,Address:%p"), this));
	}
	else if (GetRemoteRole() == ENetRole::ROLE_SimulatedProxy)
	{
		ULog::Info(*FString::Printf(TEXT("==>ASqliteCharacter::BeginPlay:RemoteRole:ROLE_SimulatedProxy,Address:%p"), this));
	}
	else if (GetRemoteRole() == ENetRole::ROLE_None)
	{
		ULog::Info(*FString::Printf(TEXT("==>ASqliteCharacter::BeginPlay:RemoteRole:ROLE_None,Address:%p"), this));
	}
	//////////////////////////////////////////////////////////////////////////
	if (GetLocalRole() == ENetRole::ROLE_Authority)
	{
		ULog::Info(*FString::Printf(TEXT("==>ASqliteCharacter::BeginPlay:LocalRole:ROLE_Authority,Address:%p"), this));
	}
	else if (GetLocalRole() == ENetRole::ROLE_AutonomousProxy)
	{
		ULog::Info(*FString::Printf(TEXT("==>ASqliteCharacter::BeginPlay:LocalRole:ROLE_AutonomousProxy,Address:%p"), this));
	}
	else if (GetLocalRole() == ENetRole::ROLE_SimulatedProxy)
	{
		ULog::Info(*FString::Printf(TEXT("==>ASqliteCharacter::BeginPlay:LocalRole:ROLE_SimulatedProxy,Address:%p"), this));
	}
	else if (GetLocalRole() == ENetRole::ROLE_None)
	{
		ULog::Info(*FString::Printf(TEXT("==>ASqliteCharacter::BeginPlay:LocalRole:ROLE_None,Address:%p"), this));
	}
}

void ASqliteCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	FScopeLog EndPlayLog(*FString::Printf(TEXT("ASqliteCharacter::EndPlay,EndPlay:%p"), this));
	ULog::Info(*FString::Printf(TEXT("==>UserId:%s,Platform:%s"), *UserId, *Platform));
	Super::EndPlay(EndPlayReason);
}

void ASqliteCharacter::BeginDestroy()
{
	FScopeLog BeginDestroyLog(*FString::Printf(TEXT("ASqliteCharacter::BeginDestroy,Address:%p"), this));
	Super::BeginDestroy();
}

void ASqliteCharacter::GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const
{
	FScopeLog GetLifetimeReplicatedPropsLog(*FString::Printf(TEXT("ASqliteCharacter::GetLifetimeReplicatedProps,Address:%p"), this));
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASqliteCharacter, UserId);
	DOREPLIFETIME(ASqliteCharacter, Platform);
}

// Called every frame
void ASqliteCharacter::Tick(float DeltaTime)
{
	//FScopeLog TickLog(*FString::Printf(TEXT("ASqliteCharacter::Tick,Address:%p"), this));
	Super::Tick(DeltaTime);
	/*UGameInstance* GameInstance = GetGameInstance();
	if (!GameInstance)
	{
		return;
	}

	if (GameInstance->IsDedicatedServerInstance())
	{
		if (GetNetConnection())
		{
			ULog::Info(TEXT("Start SqliteCharacter Close Connection!!!!"));
			GetNetConnection()->Close();
		}
		else
		{
			ULog::Error(TEXT("SqliteCharacter->GetNetConnection() is nullptr,SqliteCharacter Close Connection failed!!"));
		}
	}*/
}

// Called to bind functionality to input
void ASqliteCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	FScopeLog SetupPlayerInputComponentLog(*FString::Printf(TEXT("ASqliteCharacter::SetupPlayerInputComponent,Address:%p"), this));
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

