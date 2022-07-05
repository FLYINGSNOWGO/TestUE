// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TestUE5 : ModuleRules
{
	public TestUE5(ReadOnlyTargetRules Target) : base(Target)
	{
        PublicIncludePaths.AddRange(
                new string[] {
                    "TestUE5"
                }
                );
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "AIModule" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });
		
		PublicDefinitions.Add("V8_HAS_ATTRIBUTE_WARN_UNUSED_RESULT=0");
		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
