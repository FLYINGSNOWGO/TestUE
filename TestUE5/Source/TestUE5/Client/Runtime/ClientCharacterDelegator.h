// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/Delegate/CharacterDelegator.h"
#include "ClientCharacterDelegator.generated.h"

/**
 * 
 */
UCLASS()
class TESTUE5_API UClientCharacterDelegator : public UCharacterDelegator
{
	GENERATED_BODY()
	
public:
	UClientCharacterDelegator(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void TestCharacterDelegator() override;
	
};
