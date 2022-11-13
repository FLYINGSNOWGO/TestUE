// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/TestCaseActor.h"
#include "SVGToSplineActor.h"
#include "SVGSubsystem.h"
#include "DrawDebugHelpers.h"

DEFINE_LOG_CATEGORY(LogTestCaseActor);
// Sets default values
ATestCaseActor::ATestCaseActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

#define CREATE_SVGTOSPLINE(ChCode) \
{ \
	FString Path = FString::Printf(TEXT("SVGToSpline'/Game/Test/SVG/%d.%d'"),ChCode,ChCode); \
	USVGToSpline* SVGToSpline = LoadObject<USVGToSpline>(nullptr, *Path); \
	if (!SVGToSpline) \
	{ \
		UE_LOG(LogTestCaseActor, Error, TEXT("SVGToSpline data error")); \
	} \
	SVGToSplines.Add(SVGToSpline); \
}

// Called when the game starts or when spawned
void ATestCaseActor::BeginPlay()
{
	Super::BeginPlay();
	
	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	USVGSubsystem* SVGSubsystem = GameInstance->GetSubsystem<USVGSubsystem>();
	check(SVGSubsystem);
	SVGSubsystem->TaskComplete.AddUObject(this, &ATestCaseActor::OnTaskComplete);
	SVGSubsystem->UpdateMotionPoint.BindUObject(this, &ATestCaseActor::DoUpdateMotionPoint);
	SVGSubsystem->Start();
	
	CREATE_SVGTOSPLINE(200131);
	CREATE_SVGTOSPLINE(22269);
	CREATE_SVGTOSPLINE(20154);
	CREATE_SVGTOSPLINE(26368);
	CREATE_SVGTOSPLINE(26834);
	for (const auto &TaskData : SVGToSplines)
	{
		SVGSubsystem->AddTask(TaskData);
	}
}

void ATestCaseActor::OnTaskComplete(const EMotionStatus InMotionStatus, const FVector2D& Inpoint)
{
	UE_LOG(LogTestCaseActor, Log, TEXT("OnTaskComplete"));
}

void ATestCaseActor::DoUpdateMotionPoint(const EMotionType InMotionType, const FVector2D& LastPoint, const FVector2D& InPoint)
{
	UE_LOG(LogTestCaseActor, Log, TEXT("DoUpdateMotionPoint MotionType:%s; Point:%s"), MotionTypeToString(InMotionType), *InPoint.ToString());
	switch (InMotionType)
	{
	case EMotionType::MoveTo:
	{
		SetActorLocation(FVector(InPoint.X, InPoint.Y, 120.f));
	}
	break;
	case EMotionType::LineTo:
	{
		DrawDebugLine(GetWorld(),FVector(LastPoint.X,LastPoint.Y,120.f), FVector(InPoint.X, InPoint.Y, 120.f),FColor::Red,true,-1.f,0,30.f);
		SetActorLocation(FVector(InPoint.X, InPoint.Y, 120.f));
	}
	break;
	default:
		break;
	}
}

// Called every frame
void ATestCaseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

