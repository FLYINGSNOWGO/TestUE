// Fill out your copyright notice in the Description page of Project Settings.


#include "SqliteGameInstance.h"
#include "Engine/Engine.h"
#include "GameDelegates.h"

#include "Log.h"

void USqliteGameInstance::OnNetworkError(UWorld *World, UNetDriver *NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString)
{
	FScopeLog GameInstanceLog(*FString::Printf(TEXT("USqliteGameInstance::OnNetworkError,Address:%p"), this));
	ULog::Info(*FString::Printf(TEXT("USqliteGameInstance::OnNetworkError switch case to 'ENetworkFailure::%s'. ErrorString is %s"), ENetworkFailure::ToString(FailureType), *ErrorString));
	
	switch (FailureType)
	{
	case ENetworkFailure::Type::ConnectionLost:
	break;
	case ENetworkFailure::Type::ConnectionTimeout:
	break;
	case ENetworkFailure::Type::FailureReceived:
	break;
	case ENetworkFailure::Type::NetChecksumMismatch:
	break;
	case ENetworkFailure::Type::NetDriverAlreadyExists:
	break;
	case ENetworkFailure::Type::NetDriverCreateFailure:
	break;
	case ENetworkFailure::Type::NetDriverListenFailure:
	break;
	case ENetworkFailure::Type::NetGuidMismatch:
	break;
	case ENetworkFailure::Type::OutdatedClient:
	break;
	default:
		break;
	}
}

void USqliteGameInstance::HandleDisconnect(UWorld* InWorld, UNetDriver* NetDriver)
{
	FScopeLog GameInstanceLog(*FString::Printf(TEXT("USqliteGameInstance::HandleDisconnect,Address:%p"), this));
}

USqliteGameInstance::USqliteGameInstance(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	FScopeLog SqliteGameInstanceLog(*FString::Printf(TEXT("USqliteGameInstance::USqliteGameInstance,Address:%p"), this));
}

USqliteGameInstance::~USqliteGameInstance()
{
	FScopeLog SqliteGameInstanceLog(*FString::Printf(TEXT("USqliteGameInstance::~USqliteGameInstance,Address:%p"), this));
}

void USqliteGameInstance::PostInitProperties()
{
	FScopeLog SqliteGameInstanceLog(*FString::Printf(TEXT("USqliteGameInstance::PostInitProperties,Address:%p"), this));
	Super::PostInitProperties();
}

void USqliteGameInstance::Init()
{
	FScopeLog GameInstanceLog(*FString::Printf(TEXT("USqliteGameInstance::Init,Address:%p"), this));
	Super::Init();
	if (!IsDedicatedServerInstance())
	{
		GameInstanceDelegator.Init(this);
		GetEngine()->OnNetworkFailure().AddUObject(this, &USqliteGameInstance::OnNetworkError);
		FGameDelegates::Get().GetHandleDisconnectDelegate().AddUObject(this, &USqliteGameInstance::HandleDisconnect);
	}
}
