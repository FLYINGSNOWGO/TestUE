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
	
};
