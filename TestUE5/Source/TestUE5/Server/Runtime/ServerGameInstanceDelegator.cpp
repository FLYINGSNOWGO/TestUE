// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerGameInstanceDelegator.h"
#include "Common/NCLogChannels.h"
#include "Kismet/GameplayStatics.h"
#include "Server/Runtime/Script/TSServerEngine.h"

UServerGameInstanceDelegator::UServerGameInstanceDelegator(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/) : Super(ObjectInitializer)
{
	UE_LOG(LogServerNC, Log, TEXT("Enter UServerGameInstanceDelegator::UServerGameInstanceDelegator[%s], this:%p"), *GetClientServerContextString(this),this);
	UE_LOG(LogServerNC, Log, TEXT("Exit UServerGameInstanceDelegator::UServerGameInstanceDelegator[%s], this:%p"), *GetClientServerContextString(this),this);
}

void UServerGameInstanceDelegator::TestGameInstanceDelegator()
{
	UE_LOG(LogServerNC,Log,TEXT("Enter UServerGameInstanceDelegator::TestGameInstanceDelegator[%s], this:%p"),*GetClientServerContextString(this),this);
	UE_LOG(LogServerNC,Log,TEXT("Exit UServerGameInstanceDelegator::TestGameInstanceDelegator[%s], this:%p"),*GetClientServerContextString(this),this);
}

void UServerGameInstanceDelegator::Initialize(UGameInstance* InGameInstance)
{
	UE_LOG(LogServerNC, Log, TEXT("Enter UServerGameInstanceDelegator::Initialize[%s], this:%p"), *GetClientServerContextString(this), this);
	Super::Initialize(InGameInstance);
	UTSServerEngine::GetTSInstance()->Init(InGameInstance);
	UE_LOG(LogServerNC, Log, TEXT("Exit UServerGameInstanceDelegator::Initialize[%s], this:%p"), *GetClientServerContextString(this), this);
}

void UServerGameInstanceDelegator::OnStart()
{
	UE_LOG(LogServerNC, Log, TEXT("Enter UServerGameInstanceDelegator::OnStart[%s], this:%p"), *GetClientServerContextString(this), this);
	UTSServerEngine::GetTSInstance()->OnStart();
	UE_LOG(LogServerNC, Log, TEXT("Exit UServerGameInstanceDelegator::OnStart[%s], this:%p"), *GetClientServerContextString(this), this);
}

void UServerGameInstanceDelegator::Shutdown()
{
	UE_LOG(LogServerNC, Log, TEXT("Enter UServerGameInstanceDelegator::Shutdown[%s], this:%p"), *GetClientServerContextString(this), this);
	UTSServerEngine::GetTSInstance()->Shutdown();
	UE_LOG(LogServerNC, Log, TEXT("Exit UServerGameInstanceDelegator::Shutdown[%s], this:%p"), *GetClientServerContextString(this), this);
}

void UServerGameInstanceDelegator::LoadComplete(const float LoadTime, const FString& MapName)
{
	UE_LOG(LogServerNC, Log, TEXT("Enter UServerGameInstanceDelegator::LoadComplete[%s], this:%p"), *GetClientServerContextString(this), this);
	UTSServerEngine::GetTSInstance()->GameInstanceLoadComplete(LoadTime, MapName);
	UE_LOG(LogServerNC, Log, TEXT("Exit UServerGameInstanceDelegator::LoadComplete[%s], this:%p"), *GetClientServerContextString(this), this);
}
