// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllerDelegator.h"
#include "Common/Runtime/NCPlayerController.h"




void UPlayerControllerDelegator::Init(const TObjectPtr<ANCPlayerController> PC)
{
	PlayerController = PC;
}
