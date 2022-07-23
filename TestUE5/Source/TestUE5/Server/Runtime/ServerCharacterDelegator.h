// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/Delegate/CharacterDelegator.h"
#include "ServerCharacterDelegator.generated.h"

/**
 * 
 */
UCLASS()
class TESTUE5_API UServerCharacterDelegator : public UCharacterDelegator
{
	GENERATED_BODY()
	
public:
	UServerCharacterDelegator(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void TestCharacterDelegator() override;
	
};
