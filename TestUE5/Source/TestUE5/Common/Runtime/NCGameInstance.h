// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "NCGameInstance.generated.h"

class UGameInstanceDelegator;

/**
 * 
 */
UCLASS()
class TESTUE5_API UNCGameInstance : public UGameInstance
{
	GENERATED_BODY()

private:
	UPROPERTY()
		TObjectPtr<UGameInstanceDelegator> GameInstanceDelegator = nullptr;

public:
	virtual void Init() override;
	virtual void OnStart() override;
	virtual void Shutdown() override;
	virtual void LoadComplete(const float LoadTime, const FString& MapName);
};
