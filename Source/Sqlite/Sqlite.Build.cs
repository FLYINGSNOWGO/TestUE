// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System;
using System.IO;

public class Sqlite : ModuleRules
{
    private string GRPCPath
    {
        get { return Path.GetFullPath(Path.Combine(EngineDirectory, "Source/ThirdParty/Grpc/")); }
    }
    public Sqlite(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "SQLiteManager",
            "PakFile",
            "Debug",
            "Sockets",
            "JsonExtension",
            "Json",
            "Networking",
            "Http",
            "UMG"
            //"OnlineSubsystem",
            //"OnlineSubsystemUtils",
            //"SQLiteCore",
            //"DatabaseSupport"
        });

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "SqliteLoadingScreen",
                "Test"
                //"Grpc",
                //"zlib",
                //"OpenSSL"
            }
        );

        PublicIncludePaths.AddRange(
            new string[] {
				// ... add public include paths required here ...
                //Path.Combine(GRPCPath, "include"),
            }
            );
        bEnableExceptions = true;
        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            //string GRPCLibPath = Path.Combine(GRPCPath, "lib/Win64");
            //PublicLibraryPaths.Add(GRPCLibPath);
        }
        else if (Target.Platform == UnrealTargetPlatform.Android)
        {
            //string GRPCLibPath = Path.Combine(GRPCPath, "lib/Android");
            //PublicAdditionalLibraries.Add(GRPCLibPath);
        }
        else if (Target.Platform == UnrealTargetPlatform.Linux)
        {
            //string GRPCLibPath = Path.Combine(GRPCPath, "linux/lib");
            //PublicLibraryPaths.Add(GRPCLibPath);
            //PublicIncludePaths.AddRange(new string[] { Path.Combine(GRPCPath, "linux/include") });
        }
        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

            // Uncomment if you are using online features
            // PrivateDependencyModuleNames.Add("OnlineSubsystem");

            // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
