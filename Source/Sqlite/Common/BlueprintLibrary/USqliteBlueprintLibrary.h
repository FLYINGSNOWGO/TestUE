// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "USqliteBlueprintLibrary.generated.h"

class USqliteGameInstance;
/**
 * 
 */
UCLASS()
class SQLITE_API UUSqliteBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Exec)
		static void PlayLoadingScreen(bool bPlayUntilStopped, float PlayTime);
	UFUNCTION(BlueprintCallable, Exec)
		static void StopLoadingScreen();
	
};
