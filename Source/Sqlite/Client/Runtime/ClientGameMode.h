// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ClientGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SQLITE_API AClientGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AClientGameMode();
	// AGameModeBase start interface [10/19/2021 admin]
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	// AGameModeBase end interface [10/19/2021 admin]

	// Begin UObject Interface [11/8/2021 CarlZhou]
	virtual void BeginDestroy() override;
	// End UObject Interface [11/8/2021 CarlZhou]
protected:
	// Begin Actor Interface [11/8/2021 CarlZhou]
	virtual void BeginPlay() override;
	// End Actor Interface [11/8/2021 CarlZhou]
	
};
