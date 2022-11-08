// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "GameCheatManager.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogGameCheatManager, Log, All)


class FArrayReader;
/**
 * 
 */
UCLASS()
class TESTUE5_API UGameCheatManager : public UCheatManager
{
	GENERATED_BODY()

protected:
	FString ArrayUint82String(const FArrayReader& ArrayReaderRef);
	void Proc_JointStateControl(std::string databuf);
public:
	UFUNCTION(exec,BlueprintCallable,Category="GameCheatManager")
	void TestArrayUint82String();

	UFUNCTION(exec, BlueprintCallable, Category = "GameCheatManager")
		void TestCreateBin();
	UFUNCTION(exec, BlueprintCallable, Category = "GameCheatManager")
		void TestCodePoint();
	
};
