// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/Interface/PlayerControllerDelegateInterface.h"


/**
 * 
 */
class SQLITE_API FServerPlayerControllerDelegator : public FPlayerControllerDelegateInterface
{
public:
	FServerPlayerControllerDelegator();
	virtual ~FServerPlayerControllerDelegator();

	virtual void ServerSetStartTransform_Implementation(const FTransform& SpawnPointTransform) override;
};
