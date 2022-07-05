// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerControllerInterface.h"
#include "NCPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TESTUE5_API ANCPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TObjectPtr<UPlayerControllerInterface> PlayerControllerInterface = nullptr;
protected:
	virtual void BeginPlay() override;
public:
	ANCPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
};
