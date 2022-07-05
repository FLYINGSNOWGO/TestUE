// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerControllerInterface.h"
#include "ClientPlayerControllerDelegate.generated.h"

/**
 * 
 */
UCLASS()
class TESTUE5_API UClientPlayerControllerDelegate : public UPlayerControllerInterface
{
	GENERATED_BODY()
	
public:
	virtual void TestPlayerController() override;
};
