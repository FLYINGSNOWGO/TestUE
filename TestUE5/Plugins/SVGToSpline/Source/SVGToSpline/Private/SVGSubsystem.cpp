// Fill out your copyright notice in the Description page of Project Settings.


#include "SVGSubsystem.h"
#include "DataTaskProcessor.h"

//USVGSubsystem::USVGSubsystem(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
//	:Super(ObjectInitializer)
//{
//
//}

USVGSubsystem::USVGSubsystem(FVTableHelper& Helper)
	:Super(Helper)
{

}

USVGSubsystem::USVGSubsystem()
	: Super()
{
	
}

PRAGMA_DISABLE_DEPRECATION_WARNINGS
USVGSubsystem::~USVGSubsystem() = default;
PRAGMA_ENABLE_DEPRECATION_WARNINGS

void USVGSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void USVGSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

bool USVGSubsystem::AddTask(TObjectPtr<USVGToSpline> Task)
{
	bool bAddSuccess = false;
	if (DataTaskProcessor.IsValid())
	{
		bAddSuccess = DataTaskProcessor->AddTask(Task);
	}
	return bAddSuccess;
}

void USVGSubsystem::Start()
{
	DataTaskProcessor = MakeUnique<FDataTaskProcessor>(TaskComplete, UpdateMotionPoint);
	DataTaskProcessor->Start();
}
