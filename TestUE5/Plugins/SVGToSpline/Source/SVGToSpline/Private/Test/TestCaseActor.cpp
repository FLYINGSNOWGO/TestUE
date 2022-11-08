// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/TestCaseActor.h"
#include "SVGToSplineActor.h"

DEFINE_LOG_CATEGORY(LogTestCaseActor);
// Sets default values
ATestCaseActor::ATestCaseActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATestCaseActor::BeginPlay()
{
	Super::BeginPlay();
	SVGToSpline = LoadObject<USVGToSpline>(nullptr, TEXT("SVGToSpline'/Game/Test/SVG/200131.200131'"));
	if (!SVGToSpline)
	{
		UE_LOG(LogTestCaseActor,Error,TEXT("SVGToSpline data error"));
	}

	
}

// Called every frame
void ATestCaseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

