// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ServerGameMode.generated.h"

/**
 * https://docs.unrealengine.com/4.27/zh-CN/InteractiveExperiences/Networking/Server/
 */
UCLASS()
class SQLITE_API AServerGameMode : public AGameModeBase
{
	GENERATED_BODY()
private:
	FString DSIP;
	FString DSPort;
	int32 PlayerNum = 0;
public:
	AServerGameMode();
	// AGameModeBase start interface [10/19/2021 admin]
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	/** Returns default pawn class for given controller */
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;
	// AGameModeBase end interface [10/19/2021 admin]

	// Begin UObject Interface [11/8/2021 CarlZhou]
	virtual void BeginDestroy() override;
	// End UObject Interface [11/8/2021 CarlZhou]
protected:
	// Begin Actor Interface [11/8/2021 CarlZhou]
	virtual void BeginPlay() override;
	// End Actor Interface [11/8/2021 CarlZhou]

	// Begin AGameModeBase Interface [11/8/2021 CarlZhou]
	virtual FString InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal = TEXT("")) override;
	// End AGameModeBase Interface [11/8/2021 CarlZhou]
};
