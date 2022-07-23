// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/Delegate/GameInstanceDelegator.h"
#include "ClientGameInstanceDelegator.generated.h"

/**
 * 
 */
UCLASS()
class TESTUE5_API UClientGameInstanceDelegator : public UGameInstanceDelegator
{
	GENERATED_BODY()
	
public:
	UClientGameInstanceDelegator(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void TestGameInstanceDelegator() override;
	
};
