// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "NCPlayerController.generated.h"

class UPlayerControllerDelegator;

/**
 * 
 */
UCLASS()
class TESTUE5_API ANCPlayerController : public APlayerController
{
	GENERATED_BODY()
private:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPlayerControllerDelegator> PlayerControllerDelegator = nullptr;
protected:
	virtual void BeginPlay() override;
public:
	ANCPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
};
