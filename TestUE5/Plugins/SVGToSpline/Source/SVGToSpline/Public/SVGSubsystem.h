// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CommonType.h"
#include "SVGSubsystem.generated.h"

class USVGToSpline;
class FDataTaskProcessor;

/**
 * 
 */
UCLASS()
class SVGTOSPLINE_API USVGSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
private:
	TUniquePtr<FDataTaskProcessor> DataTaskProcessor;
public:
// 路径段驱动通知 [11/7/2022 CarlZhou]
	FNotifyUpdateMotionPoint UpdateMotionPoint;
// 任务完成通知 [11/7/2022 CarlZhou]
	FNotifyCompleteCmd TaskComplete;
public:
	//USVGSubsystem(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	USVGSubsystem();
	// This is declared so we can use TUniquePtr<FSkeletalMeshRenderData> with just a forward declare of that class
	USVGSubsystem(FVTableHelper& Helper);
	~USVGSubsystem();
	// Begin USubsystem [11/7/2022 CarlZhou]
	/** Implement this for initialization of instances of the system */
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/** Implement this for deinitialization of instances of the system */
	virtual void Deinitialize() override;
	// End USubsystem [11/7/2022 CarlZhou]
	bool AddTask(TObjectPtr<USVGToSpline> Task);
	void Start();
};
