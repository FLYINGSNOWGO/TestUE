// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Runtime/Engine/Classes/Engine/EngineTypes.h"

//#if UE_SERVER
//#include "Server/Runtime/ServerGameModeDelegate.h"
//#define GAMEMODE_DELEGATOR FServerGameModeDelegator
//#else
//#include "Client/Runtime/ClientGameModeDelegate.h"
//#define GAMEMODE_DELEGATOR FClientGameModeDelegator
//#endif

#include "SqliteGameModeBase.generated.h"

class USQLiteManagerObject;

class FCLPakFileVisitor : public IPlatformFile::FDirectoryVisitor
{
public:
	virtual bool Visit(const TCHAR* FilenameOrDirectory, bool bIsDirectory) override
	{
		if (!bIsDirectory)
		{
			Files.Add(FilenameOrDirectory);
		}
		return true;
	}

	TArray<FString> Files;
};
/**
 * 
 */
UCLASS()
class SQLITE_API ASqliteGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;

	struct FTestNew
	{
		int32 V1;
		FString V2;
		FTestNew(int32 InV1, const FString& InV2)
		{
			V1 = InV1;
			V2 = InV2;
		}
	};
	
	/** This class's native functions. */
	TArray<FTestNew> Table;

public:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Actor")
		FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Actor")
		int32 Age;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Actor")
		FString Gender;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Actor")
		float Height;
	FTimerHandle TimerHandle;

public:
	UPROPERTY(BlueprintReadOnly)
	USQLiteManagerObject* SQLiteManager = nullptr;

	UFUNCTION(BlueprintCallable, Exec)
	void RegisterDB();

	UFUNCTION(BlueprintCallable,Exec)
	void CreateTable();

	UFUNCTION(BlueprintCallable, Exec)
	void AddDataToActorsTable(FString NameVar, int32 AgeVar, FString GenderVar, float HeightVar);

	UFUNCTION(BlueprintCallable, Exec)
	void GetMyStats(FString ActorName);

	UFUNCTION(BlueprintCallable, Exec)
	void TestAsynRead();

	UFUNCTION(BlueprintCallable, Exec)
		void TestAsynReadWrite();

	UFUNCTION(BlueprintCallable, Exec)
		void TestNew();

	UFUNCTION(BlueprintCallable, Exec)
		FString TestCreateMD5FromFile(FString FilePath);

	UFUNCTION(BlueprintCallable, Exec)
		FString TestGetShortPakName(FString FilePath);

	UFUNCTION(BlueprintCallable, Exec)
		FString TestGetFileName(FString FilePath);
	UFUNCTION(BlueprintCallable, Exec)
		void TestTimer();
	UFUNCTION(BlueprintCallable, Exec)
	bool IsMountedDefaultPaks();

	UFUNCTION(BlueprintCallable, Exec)
		void TestMountPak();

	UFUNCTION(BlueprintCallable, Exec)
		void TestMountPak1();
	UFUNCTION(BlueprintCallable, Exec)
	void CreateBPActorNew();

	UFUNCTION(BlueprintCallable, Exec)
		void TestCreateBPActor();
	UFUNCTION()
	void OnNewFileAddedDelegate(const FString& FilePath);
};
