// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestCaseActor.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogTestCaseActor, Log, All);

UCLASS()
class ATestCaseActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestCaseActor();

public:
	UPROPERTY()
	TObjectPtr<class USVGToSpline> SVGToSpline;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
