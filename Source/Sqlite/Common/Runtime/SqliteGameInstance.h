// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "IPlatformFilePak.h"
#if UE_SERVER
#include "Server/Runtime/ServerGameInstanceDelegate.h"
#define GAMEINSTANCE_DELEGATOR FServerGameInstanceDelegator
#else
#include "Client/Runtime/ClientGameInstanceDelegate.h"
#define GAMEINSTANCE_DELEGATOR FClientGameInstanceDelegator
#endif
#include "SqliteGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SQLITE_API USqliteGameInstance : public UGameInstance
{
	GENERATED_BODY()	

private:
	GAMEINSTANCE_DELEGATOR GameInstanceDelegator;
	TSharedPtr<FPakPlatformFile> ServerPlatformFile;

protected:
	void OnNetworkError(UWorld *World, UNetDriver *NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);
	void HandleDisconnect(UWorld* InWorld, UNetDriver* NetDriver);
public:
	USqliteGameInstance(const FObjectInitializer& ObjectInitializer);
	~USqliteGameInstance();
	// start UObject Interface [12/27/2021 CarlZhou]
	virtual void PostInitProperties() override;
	// end UObject Interface [12/27/2021 CarlZhou]

	// start UGameInstance Interface [10/18/2021 admin]
	virtual void Init() override;
	// end UGameInstance Interface [10/18/2021 admin]
	GAMEINSTANCE_DELEGATOR GetGameInstance() {
		return GameInstanceDelegator;
	}

	FPakPlatformFile* GetClientPakPlatformFile()
	{
#if !UE_SERVER
		return GameInstanceDelegator.GetClientPakPlatformFile();
#endif
		return nullptr;
	}

	FPakPlatformFile* GetServerPakPlatformFile()
	{
		//return m_pGameInstance.ServerPakPlatformFile();
		return ServerPlatformFile.Get();
	}

};
