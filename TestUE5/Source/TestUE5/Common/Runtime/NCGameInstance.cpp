// Fill out your copyright notice in the Description page of Project Settings.


#include "NCGameInstance.h"
#include "Common/NCLogChannels.h"
#include "Common/Factory/DelegatorFactory.h"

void UNCGameInstance::Init()
{
	UE_LOG(LogNC, Log, TEXT("Enter UNCGameInstance::Init[%s]"), *GetClientServerContextString(this));
	UE_LOG(LogNC, Log, TEXT("UNCGameInstance::UNCGameInstance->IsDedicatedServerInstance:%s"),IsDedicatedServerInstance() ? TEXT("True") : TEXT("False"));

	GameInstanceDelegator = FDelegatorFactory::CreateGameInstanceDelegator(
		FDelegatorFactory::GetType(IsDedicatedServerInstance()));
	UE_LOG(LogNC, Log, TEXT("Exit UNCGameInstance::Init[%s]"), *GetClientServerContextString(this));
}

void UNCGameInstance::OnStart()
{
	UE_LOG(LogNC, Log, TEXT("Enter UNCGameInstance::OnStart[%s]"), *GetClientServerContextString(this));
	UE_LOG(LogNC, Log, TEXT("UNCGameInstance::OnStart->IsDedicatedServerInstance:%s"), IsDedicatedServerInstance() ? TEXT("True") : TEXT("False"));
	UE_LOG(LogNC, Log, TEXT("Exit UNCGameInstance::OnStart[%s]"), *GetClientServerContextString(this));
}

void UNCGameInstance::Shutdown()
{
	UE_LOG(LogNC, Log, TEXT("Enter UNCGameInstance::Shutdown[%s]"), *GetClientServerContextString(this));
	UE_LOG(LogNC, Log, TEXT("UNCGameInstance::Shutdown->IsDedicatedServerInstance:%s"), IsDedicatedServerInstance() ? TEXT("True") : TEXT("False"));
	UE_LOG(LogNC, Log, TEXT("Exit UNCGameInstance::Shutdown[%s]"), *GetClientServerContextString(this));
}
