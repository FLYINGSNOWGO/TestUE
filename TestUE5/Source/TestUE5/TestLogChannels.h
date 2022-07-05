// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

TESTUE5_API DECLARE_LOG_CATEGORY_EXTERN(LogNC, Log, All);

TESTUE5_API DECLARE_LOG_CATEGORY_EXTERN(LogClientNC, Log, All);
TESTUE5_API DECLARE_LOG_CATEGORY_EXTERN(LogServerNC, Log, All);

TESTUE5_API DECLARE_LOG_CATEGORY_EXTERN(LogClientTestLogin,Log, All);
TESTUE5_API DECLARE_LOG_CATEGORY_EXTERN(LogServerTestLogin,Log, All);

TESTUE5_API FString GetClientServerContextString(UObject* ContextObject = nullptr);