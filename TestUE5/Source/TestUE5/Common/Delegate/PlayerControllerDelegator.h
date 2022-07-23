// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "PlayerControllerDelegator.generated.h"

class ANCPlayerController;

/**
 * 
 */
UCLASS(BlueprintType)
class UPlayerControllerDelegator : public UObject
{
	GENERATED_BODY()
protected:
	TWeakObjectPtr<ANCPlayerController> PlayerController;
public:
	void Init(const TObjectPtr<ANCPlayerController> PC);
	virtual void TestPlayerController(){}
	virtual void BeginPlay(){}
};
