// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogNCCommonUtils, Log, All)
DECLARE_LOG_CATEGORY_EXTERN(LogNCCommonLibAutomationTest, Log, All)

#define UE_LOG_NCCOMMONLIB_AUTOMATIONTEST(Verbosity, Format, ...) \
{ \
	UE_LOG(LogNCCommonLibAutomationTest, Verbosity, TEXT("%s"), *FString::Printf(Format, ##__VA_ARGS__)); \
}

#define UE_CLOG_NCCOMMONLIB_AUTOMATIONTEST(Conditional, Verbosity, Format, ...) \
{ \
	UE_CLOG(Conditional, LogNCCommonLibAutomationTest, Verbosity, TEXT("%s"), *FString::Printf(Format, ##__VA_ARGS__)); \
}

class FNCCommonLibModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
