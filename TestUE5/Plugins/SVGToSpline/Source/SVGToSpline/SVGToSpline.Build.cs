// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SVGToSpline : ModuleRules
{
	public SVGToSpline(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
            }
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
				"SVGToSpline/Public/lunasvg-2.3.4",
                "SVGToSpline/Public/lunasvg-2.3.4/include",
				"SVGToSpline/Public/lunasvg-2.3.4/3rdparty/software",
				"SVGToSpline/Public/lunasvg-2.3.4/3rdparty/plutovg",
				"SVGToSpline/Public/lunasvg-2.3.4/source/",
				"SVGToSpline/Public/Tinyxml2-9.0.0"
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				// ... add other public dependencies that you statically link with here ...
				"Engine",
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				// ... add private dependencies that you statically link with here ...	
				"RHI",
				"RenderCore",
                "ImageWrapper"
            }
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
