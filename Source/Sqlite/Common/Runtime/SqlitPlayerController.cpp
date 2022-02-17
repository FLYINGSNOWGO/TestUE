// Fill out your copyright notice in the Description page of Project Settings.


#include "SqlitPlayerController.h"
#include "Common/Cheat/SqlitCheatManager.h"
#include "Log.h"
#include "Net/UnrealNetwork.h"

ASqlitPlayerController::ASqlitPlayerController(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	FScopeLog SqlitPlayerControllerLog(*FString::Printf(TEXT("ASqlitPlayerController::ASqlitPlayerController,Address:%p"), this));
	bReplicates = true;
	CheatClass = USqlitCheatManager::StaticClass();
	PlayerControllerDelegator.SetPlayerController(this);
}

ASqlitPlayerController::~ASqlitPlayerController()
{
	FScopeLog Log(*FString::Printf(TEXT("ASqlitPlayerController::~ASqlitPlayerController,Address:%p"), this));
}

void ASqlitPlayerController::BeginPlay()
{
	FScopeLog BeginPlayLog(*FString::Printf(TEXT("ASqlitPlayerController::BeginPlay,Address:%p,Name:%s"), this, *GetName()));
	//ConsoleCommand(TEXT("stat StartFile"));
	Super::BeginPlay();
	if (GetRemoteRole() == ENetRole::ROLE_Authority)
	{
		ULog::Info(*FString::Printf(TEXT("==>ASqlitPlayerController::BeginPlay:RemoteRole:ROLE_Authority,Address:%p"), this));
	}
	else if (GetRemoteRole() == ENetRole::ROLE_AutonomousProxy)
	{
		ULog::Info(*FString::Printf(TEXT("==>ASqlitPlayerController::BeginPlay:RemoteRole:ROLE_AutonomousProxy,Address:%p"), this));
	}
	else if (GetRemoteRole() == ENetRole::ROLE_SimulatedProxy)
	{
		ULog::Info(*FString::Printf(TEXT("==>ASqlitPlayerController::BeginPlay:RemoteRole:ROLE_SimulatedProxy,Address:%p"), this));
	}
	else if (GetRemoteRole() == ENetRole::ROLE_None)
	{
		ULog::Info(*FString::Printf(TEXT("==>ASqlitPlayerController::BeginPlay:RemoteRole:ROLE_None,Address:%p"), this));
	}
	//////////////////////////////////////////////////////////////////////////
	if (GetLocalRole() == ENetRole::ROLE_Authority)
	{
		ULog::Info(*FString::Printf(TEXT("==>ASqlitPlayerController::BeginPlay:LocalRole:ROLE_Authority,Address:%p"), this));
	}
	else if (GetLocalRole() == ENetRole::ROLE_AutonomousProxy)
	{
		ULog::Info(*FString::Printf(TEXT("==>ASqlitPlayerController::BeginPlay:LocalRole:ROLE_AutonomousProxy,Address:%p"), this));
	}
	else if (GetLocalRole() == ENetRole::ROLE_SimulatedProxy)
	{
		ULog::Info(*FString::Printf(TEXT("==>ASqlitPlayerController::BeginPlay:LocalRole:ROLE_SimulatedProxy,Address:%p"), this));
	}
	else if (GetLocalRole() == ENetRole::ROLE_None)
	{
		ULog::Info(*FString::Printf(TEXT("==>ASqlitPlayerController::BeginPlay:LocalRole:ROLE_None,Address:%p"), this));
	}
}

void ASqlitPlayerController::CleanupPlayerState()
{
	FScopeLog CleanupPlayerStateLog(*FString::Printf(TEXT("ASqlitPlayerController::CleanupPlayerState,Address:%p"), this));
	Super::CleanupPlayerState();
}

void ASqlitPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	FScopeLog EndPlayLog(*FString::Printf(TEXT("ASqlitPlayerController::EndPlay,Address:%p"), this));
	//ConsoleCommand(TEXT("stat StopFile"));
	ULog::Info(*FString::Printf(TEXT("==>UserId:%s,Platform:%s"),*UserId,*Platform));
	Super::EndPlay(EndPlayReason);
}

void ASqlitPlayerController::Destroyed()
{
	FScopeLog DestroyedLog(*FString::Printf(TEXT("ASqlitPlayerController::Destroyed,Address:%p"), this));
	Super::Destroyed();
}

void ASqlitPlayerController::PlayerTick(float DeltaTime)
{
	PlayerControllerDelegator.CheckClientNetwork(DeltaTime);
}

void ASqlitPlayerController::BeginDestroy()
{
	FScopeLog BeginDestroyLog(*FString::Printf(TEXT("ASqlitPlayerController::BeginDestroy,Address:%p"), this));
	Super::BeginDestroy();
}

void ASqlitPlayerController::GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const
{
	FScopeLog BeginDestroyLog(*FString::Printf(TEXT("ASqlitPlayerController::GetLifetimeReplicatedProps,Address:%p"), this));
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASqlitPlayerController, UserId);
	DOREPLIFETIME(ASqlitPlayerController, Platform);
}

void ASqlitPlayerController::ConnectDS()
{
	FScopeLog ConnectDSLog(*FString::Printf(TEXT("ASqlitPlayerController::ConnectDS,Address:%p"), this));
#if !UE_SERVER
	PlayerControllerDelegator.ConnectDSImplementation();
#endif
}
