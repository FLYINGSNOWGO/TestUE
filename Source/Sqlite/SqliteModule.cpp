// Fill out your copyright notice in the Description page of Project Settings.

#include "Sqlite.h"
#include "Modules/ModuleManager.h"

class FSqliteModule : public FDefaultGameModuleImpl
{
	virtual void StartupModule() override
	{
		UE_LOG(LogTemp,Log,TEXT("FSqliteModule::StartupModule;Address:%p"),this);
	}

	virtual void ShutdownModule() override
	{
		UE_LOG(LogTemp, Log, TEXT("FSqliteModule::ShutdownModule;Address:%p"), this);
	}

	virtual bool IsGameModule() const override
	{
		UE_LOG(LogTemp, Log, TEXT("FSqliteModule::IsGameModule;Address:%p"), this);
		return true;
	}

	virtual void PreUnloadCallback()
	{
		UE_LOG(LogTemp, Log, TEXT("FSqliteModule::PreUnloadCallback;Address:%p"), this);
	}

	virtual void PostLoadCallback()
	{
		UE_LOG(LogTemp, Log, TEXT("FSqliteModule::PostLoadCallback;Address:%p"), this);
	}

	virtual bool SupportsDynamicReloading()
	{
		UE_LOG(LogTemp, Log, TEXT("FSqliteModule::SupportsDynamicReloading;Address:%p"), this);
		return true;
	}

	virtual bool SupportsAutomaticShutdown()
	{
		UE_LOG(LogTemp, Log, TEXT("FSqliteModule::SupportsAutomaticShutdown;Address:%p"), this);
		return true;
	}
};


IMPLEMENT_PRIMARY_GAME_MODULE(FSqliteModule, Sqlite, "Sqlite" );
