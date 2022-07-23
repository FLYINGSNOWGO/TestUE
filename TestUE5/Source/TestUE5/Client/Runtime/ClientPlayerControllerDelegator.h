// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/Delegate/PlayerControllerDelegator.h"
#include "ClientPlayerControllerDelegator.generated.h"

/**
 * 
 */
UCLASS()
class TESTUE5_API UClientPlayerControllerDelegator : public UPlayerControllerDelegator
{
	GENERATED_BODY()
	
public:
	UClientPlayerControllerDelegator(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void TestPlayerController() override;
	virtual void BeginPlay() override;
public:
	UFUNCTION(BlueprintCallable)
		void ConnectDS();
};
