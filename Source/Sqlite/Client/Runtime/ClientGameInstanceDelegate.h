// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class FPakPlatformFile;
class USqliteGameInstance;

/**
 * 
 */
class SQLITE_API FClientGameInstanceDelegator
{
private:
	TSharedPtr<FPakPlatformFile> ClientPakPlatformFile;
public:
	FClientGameInstanceDelegator();
	virtual ~FClientGameInstanceDelegator();
	virtual void Init(USqliteGameInstance *GameInstance);
	FPakPlatformFile* GetClientPakPlatformFile();
};
