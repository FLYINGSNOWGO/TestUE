// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientGameInstanceDelegator.h"
#include "Common/NCLogChannels.h"
#include "Kismet/GameplayStatics.h"
#include "Client/Runtime/Script/TSClientEngine.h"
#include "GameDelegates.h"

void UClientGameInstanceDelegator::OnNetworkError(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString)
{
	UE_LOG(LogClientNC, Log, TEXT("Enter UClientGameInstanceDelegator::OnNetworkError[%s], this:%p"), *GetClientServerContextString(this), this);
	UE_LOG(LogClientNC, Log, TEXT("UREGameInstance::OnNetworkError switch case to 'ENetworkFailure::%s'. ErrorString is %s"), ENetworkFailure::ToString(FailureType), *ErrorString);
	UTSClientEngine::GetTSInstance()->OnNetworkError(World, NetDriver, FailureType, ErrorString);
	UE_LOG(LogClientNC, Log, TEXT("Exit UClientGameInstanceDelegator::OnNetworkError[%s], this:%p"), *GetClientServerContextString(this), this);
}

void UClientGameInstanceDelegator::HandleDisconnect(UWorld* InWorld, UNetDriver* NetDriver)
{
	UE_LOG(LogClientNC, Log, TEXT("Enter UClientGameInstanceDelegator::HandleDisconnect[%s], this:%p"), *GetClientServerContextString(this), this);
	UTSClientEngine::GetTSInstance()->HandleDisconnect(InWorld, NetDriver);
	UE_LOG(LogClientNC, Log, TEXT("Exit UClientGameInstanceDelegator::HandleDisconnect[%s], this:%p"), *GetClientServerContextString(this), this);
}

UClientGameInstanceDelegator::UClientGameInstanceDelegator(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/) : Super(ObjectInitializer)
{
	UE_LOG(LogClientNC, Log, TEXT("Enter UClientGameInstanceDelegator::UClientGameInstanceDelegator[%s], this:%p"), *GetClientServerContextString(this),this);
	UE_LOG(LogClientNC, Log, TEXT("Exit UClientGameInstanceDelegator::UClientGameInstanceDelegator[%s], this:%p"), *GetClientServerContextString(this),this);
}

void UClientGameInstanceDelegator::TestGameInstanceDelegator()
{
	UE_LOG(LogClientNC,Log,TEXT("Enter UClientGameInstanceDelegator::TestGameInstanceDelegator[%s], this:%p"),*GetClientServerContextString(this),this);
	UE_LOG(LogClientNC,Log,TEXT("Exit UClientGameInstanceDelegator::TestGameInstanceDelegator[%s], this:%p"),*GetClientServerContextString(this),this);
}

void UClientGameInstanceDelegator::Initialize(UGameInstance* InGameInstance)
{
	UE_LOG(LogClientNC, Log, TEXT("Enter UClientGameInstanceDelegator::Initialize[%s], this:%p"), *GetClientServerContextString(this), this);
	Super::Initialize(InGameInstance);
	GameInstance->GetEngine()->OnNetworkFailure().AddUObject(this, &UClientGameInstanceDelegator::OnNetworkError);
	FGameDelegates::Get().GetHandleDisconnectDelegate().AddUObject(this, &UClientGameInstanceDelegator::HandleDisconnect);
	UTSClientEngine::GetTSInstance()->Init(GameInstance);
	UE_LOG(LogClientNC, Log, TEXT("Exit UClientGameInstanceDelegator::Initialize[%s], this:%p"), *GetClientServerContextString(this), this);
}

void UClientGameInstanceDelegator::OnStart()
{
	UE_LOG(LogClientNC, Log, TEXT("Enter UClientGameInstanceDelegator::OnStart[%s], this:%p"), *GetClientServerContextString(this), this);
	UTSClientEngine::GetTSInstance()->OnStart();
	UE_LOG(LogClientNC, Log, TEXT("Exit UClientGameInstanceDelegator::OnStart[%s], this:%p"), *GetClientServerContextString(this), this);
}

void UClientGameInstanceDelegator::Shutdown()
{
	UE_LOG(LogClientNC, Log, TEXT("Enter UClientGameInstanceDelegator::Shutdown[%s], this:%p"), *GetClientServerContextString(this), this);
	UTSClientEngine::GetTSInstance()->Shutdown();
	UE_LOG(LogClientNC, Log, TEXT("Exit UClientGameInstanceDelegator::Shutdown[%s], this:%p"), *GetClientServerContextString(this), this);
}

void UClientGameInstanceDelegator::LoadComplete(const float LoadTime, const FString& MapName)
{
	UE_LOG(LogClientNC, Log, TEXT("Enter UClientGameInstanceDelegator::LoadComplete[%s], this:%p"), *GetClientServerContextString(this), this);
	UTSClientEngine::GetTSInstance()->GameInstanceLoadComplete(LoadTime, MapName);;
	UE_LOG(LogClientNC, Log, TEXT("Exit UClientGameInstanceDelegator::LoadComplete[%s], this:%p"), *GetClientServerContextString(this), this);
}
