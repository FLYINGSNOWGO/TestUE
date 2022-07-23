// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerGameMode.h"
#include "Common/NCLogChannels.h"


FString AServerGameMode::InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId,
	const FString& Options, const FString& Portal)
{
	UE_LOG(LogServerTestLogin,Log,TEXT("Enter AServerGameMode::InitNewPlayer[%s]"),*GetClientServerContextString(this));
	return Super::InitNewPlayer(NewPlayerController, UniqueId, Options, Portal);
	UE_LOG(LogServerTestLogin,Log,TEXT("Exit AServerGameMode::InitNewPlayer[%s]"),*GetClientServerContextString(this));
}

void AServerGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId,
                               FString& ErrorMessage)
{
	UE_LOG(LogServerTestLogin,Log,TEXT("Enter AServerGameMode::PreLogin[%s]"),*GetClientServerContextString(this));
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
	UE_LOG(LogServerTestLogin,Log,TEXT("Exit AServerGameMode::PreLogin[%s]"),*GetClientServerContextString(this));
}

void AServerGameMode::PostLogin(APlayerController* NewPlayer)
{
	UE_LOG(LogServerTestLogin,Log,TEXT("Enter AServerGameMode::PostLogin[%s]"),*GetClientServerContextString(this));
	Super::PostLogin(NewPlayer);
	UE_LOG(LogServerTestLogin,Log,TEXT("Exit AServerGameMode::PostLogin[%s]"),*GetClientServerContextString(this));
}
