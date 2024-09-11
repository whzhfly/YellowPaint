// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class YellowPaint : ModuleRules
{
	public YellowPaint(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		OptimizeCode = CodeOptimization.Never; // todo
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
			"Core",
			"CoreUObject",
			"Engine",
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
			"Core",
			"CoreUObject",
			"Engine",
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
