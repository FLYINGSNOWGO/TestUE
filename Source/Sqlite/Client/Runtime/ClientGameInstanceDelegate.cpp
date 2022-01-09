// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientGameInstanceDelegate.h"
#include "HAL/PlatformFilemanager.h"
#include "IPlatformFilePak.h"
#include "Common/Runtime/SqliteGameInstance.h"
#include "Log.h"

FClientGameInstanceDelegator::FClientGameInstanceDelegator()
{
	FScopeLog GameInstanceDelegatorLog(TEXT("FClientGameInstanceDelegator::FClientGameInstanceDelegator"),ELoggingOptions::LO_Console);
}

FClientGameInstanceDelegator::~FClientGameInstanceDelegator()
{
	//FScopeLog GameInstanceDelegatorLog(TEXT("FClientGameInstanceDelegator::~FClientGameInstanceDelegator"));
}

void FClientGameInstanceDelegator::Init(USqliteGameInstance *GameInstance)
{
	FScopeLog GameInstanceDelegatorLog(TEXT("FClientGameInstanceDelegator::Init"));
	ULog::Info(FString::Printf(TEXT("FPlatformFileManager::Get().GetPlatformFile Address:%p;"), &FPlatformFileManager::Get().GetPlatformFile()), ELoggingOptions::LO_Both);
	ULog::Info(FString::Printf(TEXT("FPlatformFileManager::Get().GetPlatformFilePakFile Address:%p;"), FPlatformFileManager::Get().GetPlatformFile(TEXT("PakFile"))), ELoggingOptions::LO_Both);
	ClientPakPlatformFile = MakeShared<FPakPlatformFile>();
	// 使用下面的语句将会使自定义的PakFile无法获取基础pak [10/26/2021 admin]
	//ClientPakPlatformFile->Initialize(&FPlatformFileManager::Get().GetPlatformFile(), TEXT(""));
	// 正确写法 [10/26/2021 admin]
	//ClientPakPlatformFile->Initialize(&IPlatformFile::GetPlatformPhysical(), TEXT(""));
	// 正确写法 [10/26/2021 admin]
#if UE_BUILD_SHIPPING

#if PLATFORM_ANDROID
	ClientPakPlatformFile->Initialize(FPlatformFileManager::Get().GetPlatformFile(TEXT("PakFile"))->GetLowerLevel(), TEXT(""));
#else
	ClientPakPlatformFile->Initialize(&FPlatformFileManager::Get().GetPlatformFile(), TEXT(""));
#endif
	FPlatformFileManager::Get().SetPlatformFile(*ClientPakPlatformFile);
#endif
}


FPakPlatformFile* FClientGameInstanceDelegator::GetClientPakPlatformFile()
{
	FScopeLog GameInstanceDelegatorLog(TEXT("FClientGameInstanceDelegator::GetClientPakPlatformFile"));
	return ClientPakPlatformFile.Get();
}
