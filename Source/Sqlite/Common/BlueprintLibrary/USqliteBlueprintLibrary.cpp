// Fill out your copyright notice in the Description page of Project Settings.


#include "USqliteBlueprintLibrary.h"
#include "Common/Runtime/SqliteGameInstance.h"
#include "SqliteLoadingScreen.h"
#include "Kismet/GameplayStatics.h"

void UUSqliteBlueprintLibrary::PlayLoadingScreen(bool bPlayUntilStopped, float PlayTime)
{
	ISqliteLoadingScreenModule& LoadingScreenModule = ISqliteLoadingScreenModule::Get();
	LoadingScreenModule.StartInGameLoadingScreen(bPlayUntilStopped, PlayTime);
}

void UUSqliteBlueprintLibrary::StopLoadingScreen()
{
	UE_LOG(LogTemp,Display,TEXT("UUSqliteBlueprintLibrary::StopLoadingScreen"));
	ISqliteLoadingScreenModule& LoadingScreenModule = ISqliteLoadingScreenModule::Get();
	LoadingScreenModule.StopInGameLoadingScreen();
}
