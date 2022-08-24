// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GameInstanceDelegator.generated.h"

class UGameInstance;

/**
 * 
 */
UCLASS()
class UGameInstanceDelegator : public UObject
{
	GENERATED_BODY()
protected:
	TWeakObjectPtr<UGameInstance> GameInstance;
public:
	virtual void TestGameInstanceDelegator(){}
	virtual void Initialize(UGameInstance* InGameInstance);
	virtual void OnStart(){}
	virtual void Shutdown() {}
	virtual void LoadComplete(const float LoadTime, const FString& MapName) {}
};
