// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "SqlitCheatManager.generated.h"

class USqliteGameInstance;

/**
 * 
 */
UCLASS()
class SQLITE_API USqlitCheatManager : public UCheatManager
{
	GENERATED_BODY()


public:
	UFUNCTION(exec, BlueprintCallable, Category = "Sqlite Cheat Manager")
		void TestMountTestMapPak();
	UFUNCTION(exec, BlueprintCallable, Category = "Sqlite Cheat Manager")
		void TestUseTestMapPak();
	UFUNCTION(exec, BlueprintCallable, Category = "Sqlite Cheat Manager")
		void TestLoadMap();
	UFUNCTION(exec, BlueprintCallable, Category = "Sqlite Cheat Manager")
		void TestMountTestMapPakUseCustomPakFile();
	UFUNCTION(exec, BlueprintCallable, Category = "Sqlite Cheat Manager")
		void TestUseTestMapPakByUseCustomPakFile();

	UFUNCTION(exec, BlueprintCallable, Category = "Sqlite Cheat Manager")
		void TestPaths();

	UFUNCTION(exec, BlueprintCallable, Category = "Sqlite Cheat Manager")
		void TestExit();
	/**
	*	获取安装的GameContentDir
	*/
	UFUNCTION(exec, BlueprintCallable, Category = "Sqlite Cheat Manager")
		void TestInstalledGameContentDir();

	UFUNCTION(exec, BlueprintCallable, Category = "Sqlite Cheat Manager")
		void TestPlatformFileAddress();
	UFUNCTION(exec, BlueprintCallable, Category = "Sqlite Cheat Manager")
		void TestCrashDump();

	UFUNCTION(exec, BlueprintCallable, Category = "Test DS")
		void TestCharacterInfo();
	UFUNCTION(exec, BlueprintCallable, Category = "Test DS")
		void TestCloseNetConnection();
	UFUNCTION(exec, BlueprintCallable, Category = "Test DS")
		void TestJsonExtension();
	UFUNCTION(exec, BlueprintCallable, Category = "Test DS")
		void TestTMap();
	UFUNCTION(exec, BlueprintCallable, Category = "Test DS")
		void TestCharacter();
	UFUNCTION(exec, BlueprintCallable, Category = "Test DS")
		void TestABTesting();
	UFUNCTION(exec, BlueprintCallable, Category = "Test DS")
		void TestHttp(FString Url);
	UFUNCTION(exec, BlueprintCallable, Category = "Test DS")
		void AsynShowMessageBox(FString Msg);
	UFUNCTION(exec, BlueprintCallable, Category = "Test DS")
		void TestAsynCreateUI();
	UFUNCTION(exec, BlueprintCallable, Category = "Test DS")
		void TestStringToFString();
	UFUNCTION(exec, BlueprintCallable, Category = "Test DS")
		void TestArraySort();
	UFUNCTION(exec, BlueprintCallable, Category = "Test Event Thread")
		void SetupTestEventThread();
	UFUNCTION(exec, BlueprintCallable, Category = "Test Event Thread")
		void TestEventThreadSuspend(bool bShouldSuspend);
	UFUNCTION(exec, BlueprintCallable, Category = "Test Event Thread")
		void TestEventThreadExit();
	UFUNCTION(exec, BlueprintCallable, Category = "Test Event Thread")
		void TestFEvent();
	UFUNCTION(exec, BlueprintCallable, Category = "Test Config")
		void TestGameUserSettingConfig();
	UFUNCTION(exec, BlueprintCallable, Category = "Sqlite Cheat Manager")
		void TestGetEngineVersion();
	UFUNCTION(exec, BlueprintCallable, Category = "Test Download Manager")
		void TestDownloadManager();
	UFUNCTION(exec, BlueprintCallable, Category = "Test Download Manager")
		void TestGetCurLevelName();
	UFUNCTION(exec, BlueprintCallable, Category = "Test Download Manager")
		void TestCheck();
	UFUNCTION(exec, BlueprintCallable, Category = "Test RPC")
		void TestRPC();
	UFUNCTION(exec, BlueprintCallable, Category = "Test Log")
		void TestLog();
	UFUNCTION(exec, BlueprintCallable, Category = "Test Log")
		void TestT2U();
	UFUNCTION(exec, BlueprintCallable, Category = "Test ConsoleVariable")
		void TestConsoleVariable();
	UFUNCTION(exec, BlueprintCallable, Category = "Test Number")
		void TestNumber();
public:
	UFUNCTION()
		void OnClientStreamingLevelLoaded(int32 Linkage);
	UFUNCTION()
		void OnClientStreamingLevelLoadedByUseCustomPakFile(int32 Linkage);

private:
	void SetString(FString& OutString);
	
};
