// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class USqliteGameInstance;
/**
 * 
 */
class SQLITE_API FServerGameInstanceDelegator
{
public:
	FServerGameInstanceDelegator();
	~FServerGameInstanceDelegator();

	void Init(USqliteGameInstance *GameInstance);
};
