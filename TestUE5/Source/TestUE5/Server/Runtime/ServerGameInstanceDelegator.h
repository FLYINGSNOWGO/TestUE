// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/Delegate/GameInstanceDelegator.h"
#include "ServerGameInstanceDelegator.generated.h"

/**
 * 
 */
UCLASS()
class TESTUE5_API UServerGameInstanceDelegator : public UGameInstanceDelegator
{
	GENERATED_BODY()
	
public:
	UServerGameInstanceDelegator(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void TestGameInstanceDelegator() override;

	virtual void Initialize(UGameInstance* InGameInstance) override;
	virtual void OnStart() override;
	virtual void Shutdown() override;
	virtual void LoadComplete(const float LoadTime, const FString& MapName);
	
};
