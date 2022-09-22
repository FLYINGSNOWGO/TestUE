// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TSDelegateDeclare.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNotifyWithFloat, float, A);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNotifyWithNothing);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FNotifyWithNetErr, ENetworkFailure::Type, code, FString, msg);

DECLARE_DYNAMIC_DELEGATE_OneParam(FLevelLoadCompletedDelgate, int32, A);
DECLARE_DYNAMIC_DELEGATE_OneParam(FLevelLoadBeginDelgate, const FString&, A);
DECLARE_DYNAMIC_DELEGATE_OneParam(FLevelLoadEndDelgate, UWorld*, A);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FGameInstanceLoadCompleteDelegate, float, loadTime, const FString&, mapName);
DECLARE_DYNAMIC_DELEGATE_RetVal_TwoParams(bool, FHitResltSort, const FHitResult&, A, const FHitResult&, B);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEventToTSDeleagte, int, EventCmd, const FString&, Params);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FNotifyToTSDeleagte, const FString&, A, const FString&, B);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRobotMessageDeleagte, const FString&, msgId, const FString&, data);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnRobotInfoDeleagte, const FString&, robotId, const FString&, robotType, const FString&, robotModel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FActorBeginEndPlayToTSDeleagte, const AActor*, A);

/**
 * 
 */
UCLASS()
class TESTUE5_API UTSDelegateDeclare : public UObject
{
	GENERATED_BODY()
	
};
