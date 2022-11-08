// Fill out your copyright notice in the Description page of Project Settings.


#include "SVGSubsystem.h"
#include "DataTaskProcessor.h"

void USVGSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	DataTaskProcessor = MakeUnique<FDataTaskProcessor>();
	DataTaskProcessor->Start();
}

void USVGSubsystem::Deinitialize()
{

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
