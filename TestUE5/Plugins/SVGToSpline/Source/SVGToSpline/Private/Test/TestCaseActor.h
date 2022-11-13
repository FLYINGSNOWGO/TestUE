// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CommonType.h"
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
	TArray<TObjectPtr<class USVGToSpline>> SVGToSplines;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void OnTaskComplete(const EMotionStatus InMotionStatus, const FVector2D& Inpoint);
	void DoUpdateMotionPoint(const EMotionType InMotionType, const FVector2D& LastPoint, const FVector2D& InPoint);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
