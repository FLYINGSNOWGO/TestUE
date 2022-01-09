// (c) 2017 Damian Nowakowski

using System;
using System.IO;
using UnrealBuildTool;

public class CrashDump : ModuleRules
{
	public CrashDump(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(new string[] {"CrashDump/Public"});
		PrivateIncludePaths.AddRange(new string[] {"CrashDump/Private"});		
		PublicDependencyModuleNames.AddRange(new string[]{"Core", "CoreUObject","Engine"});

        Console.WriteLine("ModulePath:" + Path.Combine(ModuleDirectory, "CrashDump_UPL_Android.xml"));

        //if (Target.Platform == UnrealTargetPlatform.Android && 
        if (Target.IsInPlatformGroup(UnrealPlatformGroup.Android) &&
            Target.Configuration != UnrealTargetConfiguration.Shipping &&
            Target.Configuration != UnrealTargetConfiguration.Test)
        {
            PublicDefinitions.Add("WITH_CRASH_DUMP=1");
            PrivateDependencyModuleNames.AddRange(new string[] { "Launch" });

            //string PluginPath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);
            Console.WriteLine("ModulePath:" + Path.Combine(ModuleDirectory, "CrashDump_UPL_Android.xml"));
			AdditionalPropertiesForReceipt.Add("AndroidPlugin", Path.Combine(ModuleDirectory, "CrashDump_UPL_Android.xml"));
            string ThirdPartyPath = Path.Combine(ModuleDirectory, "..", "ThirdParty");
            PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "GoogleBreakpad", "include"));
            PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "GoogleBreakpad", "libbreakpad_client.a"));
        }
        else
        {
            PublicDefinitions.Add("WITH_CRASH_DUMP=0");
        }

    }
}
