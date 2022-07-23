// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/Delegate/PlayerControllerDelegator.h"
#include "ServerPlayerControllerDelegator.generated.h"

/**
 * 
 */
UCLASS()
class TESTUE5_API UServerPlayerControllerDelegator : public UPlayerControllerDelegator
{
	GENERATED_BODY()
	
public:
	UServerPlayerControllerDelegator(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void TestPlayerController() override;
	
};
