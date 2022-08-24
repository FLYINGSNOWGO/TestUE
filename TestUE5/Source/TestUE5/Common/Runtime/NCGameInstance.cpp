// Fill out your copyright notice in the Description page of Project Settings.


#include "NCGameInstance.h"
#include "Common/NCLogChannels.h"
#include "Common/Factory/DelegatorFactory.h"
#include "Common/Delegate/GameInstanceDelegator.h"

void UNCGameInstance::Init()
{
	UE_LOG(LogNC, Log, TEXT("Enter UNCGameInstance::Init[%s], this:%p"), *GetClientServerContextString(this),this);
	UE_LOG(LogNC, Log, TEXT("UNCGameInstance::UNCGameInstance->IsDedicatedServerInstance:%s"),IsDedicatedServerInstance() ? TEXT("True") : TEXT("False"));

	GameInstanceDelegator = FDelegatorFactory::CreateGameInstanceDelegator(
		FDelegatorFactory::GetType(IsDedicatedServerInstance()));

	if (!IsDedicatedServerInstance()) 
	{
		GameInstanceDelegator->Initialize(this);
	}
	else 
	{
		GameInstanceDelegator->Initialize(this);
	}


	UE_LOG(LogNC, Log, TEXT("Exit UNCGameInstance::Init[%s], this:%p"), *GetClientServerContextString(this),this);
}

void UNCGameInstance::OnStart()
{
	UE_LOG(LogNC, Log, TEXT("Enter UNCGameInstance::OnStart[%s], this:%p"), *GetClientServerContextString(this),this);
	UE_LOG(LogNC, Log, TEXT("UNCGameInstance::OnStart->IsDedicatedServerInstance:%s"), IsDedicatedServerInstance() ? TEXT("True") : TEXT("False"));
	GameInstanceDelegator->OnStart();
	UE_LOG(LogNC, Log, TEXT("Exit UNCGameInstance::OnStart[%s], this:%p"), *GetClientServerContextString(this),this);
}

void UNCGameInstance::Shutdown()
{
	UE_LOG(LogNC, Log, TEXT("Enter UNCGameInstance::Shutdown[%s],this:%p"), *GetClientServerContextString(this),this);
	UE_LOG(LogNC, Log, TEXT("UNCGameInstance::Shutdown->IsDedicatedServerInstance:%s"), IsDedicatedServerInstance() ? TEXT("True") : TEXT("False"));
	GameInstanceDelegator->Shutdown();
	UE_LOG(LogNC, Log, TEXT("Exit UNCGameInstance::Shutdown[%s], this:%p"), *GetClientServerContextString(this),this);
}

void UNCGameInstance::LoadComplete(const float LoadTime, const FString& MapName)
{
	UE_LOG(LogNC, Log, TEXT("Enter UNCGameInstance::LoadComplete[%s],this:%p"), *GetClientServerContextString(this), this);
	UE_LOG(LogNC, Log, TEXT("UNCGameInstance::LoadComplete->IsDedicatedServerInstance:%s"), IsDedicatedServerInstance() ? TEXT("True") : TEXT("False"));
	GameInstanceDelegator->LoadComplete(LoadTime, MapName);
	UE_LOG(LogNC, Log, TEXT("Exit UNCGameInstance::LoadComplete[%s], this:%p"), *GetClientServerContextString(this), this);
}
