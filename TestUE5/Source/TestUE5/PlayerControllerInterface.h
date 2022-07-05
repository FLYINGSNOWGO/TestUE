// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "PlayerControllerInterface.generated.h"

/**
 * 
 */
UCLASS()
class UPlayerControllerInterface : public UObject
{
	GENERATED_BODY()
public:
	virtual void TestPlayerController(){}
};
