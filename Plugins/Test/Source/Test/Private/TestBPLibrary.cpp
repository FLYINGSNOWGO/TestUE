// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestBPLibrary.h"
#include "Test.h"

UTestBPLibrary::UTestBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

float UTestBPLibrary::TestSampleFunction(float Param)
{
	return -1;
}

