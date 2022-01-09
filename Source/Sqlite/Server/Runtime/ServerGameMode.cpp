// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerGameMode.h"
#include "Common/Runtime/SqlitPlayerController.h"
#include "Common/Runtime/SqlitGameState.h"
#include "Common/Runtime/SqliteCharacter.h"
#include "Log.h"
#include "IPAddress.h"
#include "SocketSubsystem.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/NetConnection.h"

AServerGameMode::AServerGameMode()
{
	FScopeLog ServerGameModeLog(*FString::Printf(TEXT("AServerGameMode::AServerGameMode,Address:%p"), this));
	DefaultPawnClass = NULL;// PlayerPawnBPClass.Class;
	PlayerControllerClass = ASqlitPlayerController::StaticClass();
	GameStateClass = ASqlitGameState::StaticClass();

	PrimaryActorTick.bCanEverTick = true;

	bool bCanBindAll = false;
	TSharedRef<FInternetAddr> LocalIp = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLocalHostAddr(*GLog, bCanBindAll);

	if (LocalIp->IsValid())
	{
		DSIP = LocalIp->ToString(false);
		ULog::Info(*FString::Printf(TEXT("LocalIp:%s"), *DSIP));
	}
}

void AServerGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	FScopeLog InitGameLog(*FString::Printf(TEXT("AServerGameMode::InitGame,Address:%p"), this));
	ULog::Info(*FString::Printf(TEXT("MapName:%s,Options:%s,ErrorMessage:%s,GetWorld()->URL.Port:"),*MapName,*Options,*ErrorMessage, GetWorld()->URL.Port));
	DSPort = FString::FromInt(GetWorld()->URL.Port);
	ULog::Info(*FString::Printf(TEXT("Ip:%s,Port:%s"), *DSIP, *DSPort));
	bUseSeamlessTravel = true;
	Super::InitGame(MapName, Options, ErrorMessage);
	ErrorMessage = TEXT("Test Init ErrorMessage");
}

void AServerGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	FScopeLog PreLoginLog(*FString::Printf(TEXT("AServerGameMode::PreLogin,Address:%p"), this));
	ULog::Info(TEXT("Called after the client loads the map"));
	ULog::Info(*FString::Printf(TEXT("Options:%s,Address:%s,ErrorMessage:%s"), *Options, *Address, *ErrorMessage));

	FString UserId = UGameplayStatics::ParseOption(Options, TEXT("UserId")).TrimStart();
	FString Platform = UGameplayStatics::ParseOption(Options, TEXT("Platform")).TrimStart();

	ULog::Info(*FString::Printf(TEXT("UserId:%s,Platform:%s"), *UserId, *Platform));

	TArray<AActor*> OutAllPlayerControllers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASqlitPlayerController::StaticClass(), OutAllPlayerControllers);
	ULog::Info(FString::Printf(TEXT("ASqliteCharacter Num:%d"), OutAllPlayerControllers.Num()));
	for (auto& ItemPC : OutAllPlayerControllers)
	{
		ULog::Info(FString::Printf(TEXT("PlayerController Name:%s,Address:%p"), *ItemPC->GetFullName(),ItemPC));
		//ASqlitPlayerController* PlayerController = Cast<ASqlitPlayerController>(ItemPC);
	}

	//ErrorMessage = TEXT("==>Notify Test Error Message!");
}

void AServerGameMode::PostLogin(APlayerController* NewPlayer)
{
	FScopeLog PostLoginLog(*FString::Printf(TEXT("AServerGameMode::PostLogin,Address:%p"), this));
	ULog::Info(TEXT("After this function is called, RPC is safe to use"));
	UGameInstance* GameInstance = GetGameInstance();
	if (!GameInstance)
	{
		ULog::Info(*FString::Printf(TEXT("Game Instance Is Invalid!!")));
		//TODO: The client should be notified to reconnect [11/10/2021 CarlZhou]
		return;
	}

	if (GameInstance->IsDedicatedServerInstance())
	{
		ASqlitPlayerController* PlayerController = Cast<ASqlitPlayerController>(NewPlayer);
		if (PlayerController)
		{
			UWorld* World = GetWorld();
			if (World)
			{
				FActorSpawnParameters ActorSpawnParameters = FActorSpawnParameters();
				ActorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
				ActorSpawnParameters.bNoFail = true;
				ASqliteCharacter* SqliteCharacter = World->SpawnActor<ASqliteCharacter>(ActorSpawnParameters);
				if (!SqliteCharacter)
				{
					ULog::Error(TEXT("SpawnActor Failed,Character Created Failed!!"));
				}
				else
				{
					PlayerController->Possess(SqliteCharacter);
					SqliteCharacter->UserId = PlayerController->UserId;
					SqliteCharacter->Platform = PlayerController->Platform;
					/*ULog::Info(TEXT("Start PlayerController Close Connection!!"));
					if (SqliteCharacter->GetNetConnection())
					{
						SqliteCharacter->GetNetConnection()->Close();
					}
					else
					{
						ULog::Error(TEXT("SqliteCharacter->GetNetConnection() is nullptr,SqliteCharacter Close Connection failed!!"));
					}*/
				}
			}
			else
			{
				ULog::Error(TEXT("World Is Invalid,Character Created Failed!!"));
			}
		}
	}
}

void AServerGameMode::BeginDestroy()
{
	FScopeLog BeginDestroyLog(*FString::Printf(TEXT("AServerGameMode::BeginDestroy,Address:%p"), this));
	Super::BeginDestroy();
}

void AServerGameMode::BeginPlay()
{
	FScopeLog BeginPlayLog(*FString::Printf(TEXT("AServerGameMode::BeginPlay,Address:%p"), this));
	Super::BeginPlay();
}

FString AServerGameMode::InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal /*= TEXT("")*/)
{
	FScopeLog InitNewPlayerLog(*FString::Printf(TEXT("AServerGameMode::InitNewPlayer,Address:%p"), this));
	FString Rs = Super::InitNewPlayer(NewPlayerController, UniqueId, Options, Portal);
	FString UserId = UGameplayStatics::ParseOption(Options, TEXT("UserId")).TrimStart();
	FString Platform = UGameplayStatics::ParseOption(Options, TEXT("Platform")).TrimStart();

	ULog::Info(*FString::Printf(TEXT("Rs:%s,UserId:%s,Platform:%s"), *Rs,*UserId, *Platform));
	if (ASqlitPlayerController* PlayerController = Cast<ASqlitPlayerController>(NewPlayerController))
	{
		PlayerController->UserId = UserId;
		PlayerController->Platform = Platform;
	}

	return Rs;
}
