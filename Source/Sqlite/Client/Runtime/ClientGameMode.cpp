// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientGameMode.h"
#include "Log.h"
#include "Common/Runtime/SqlitPlayerController.h"
#include "Common/Runtime/SqliteCharacter.h"

AClientGameMode::AClientGameMode()
{
	FScopeLog ClientGameModeLog(*FString::Printf(TEXT("AClientGameMode::AClientGameMode,Address:%p"), this));
	PlayerControllerClass = ASqlitPlayerController::StaticClass();
	DefaultPawnClass = ASqliteCharacter::StaticClass();
}

void AClientGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	FScopeLog InitGameLog(*FString::Printf(TEXT("AClientGameMode::InitGame,Address:%p"), this));
	bUseSeamlessTravel = true;
	Super::InitGame(MapName, Options, ErrorMessage);
}

void AClientGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	FScopeLog PreLoginLog(*FString::Printf(TEXT("AClientGameMode::PreLogin,Address:%p"), this));
}

void AClientGameMode::PostLogin(APlayerController* NewPlayer)
{
	FScopeLog PostLoginLog(*FString::Printf(TEXT("AClientGameMode::PostLogin,Address:%p"), this));
}

void AClientGameMode::BeginDestroy()
{
	FScopeLog BeginDestroyLog(*FString::Printf(TEXT("AClientGameMode::BeginDestroy,Address:%p"), this));
	Super::BeginDestroy();
}

void AClientGameMode::BeginPlay()
{
	FScopeLog BeginPlayLog(*FString::Printf(TEXT("AClientGameMode::BeginPlay:%p"), this));
	Super::BeginPlay();
}