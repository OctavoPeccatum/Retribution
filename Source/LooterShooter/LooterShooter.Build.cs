// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class LooterShooter : ModuleRules
{
	public LooterShooter(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] 
		{	"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"HeadMountedDisplay", 
			"EnhancedInput",
			"SlateCore",
			"Slate", 
			"UMG"
		});

		PublicIncludePaths.AddRange(new string[]
		{   "LooterShooter/Public/Components",
			"LooterShooter/Public/Controllers",
			"LooterShooter/Public/Data",
			"LooterShooter/Public/Interfaces",
			"LooterShooter/Public/Items",
			"LooterShooter/Public/Quests",
			"LooterShooter/Public/UserInterfaces",
            "LooterShooter/Public/Dev",
            "LooterShooter/Public/Weapon"
        });
	}
}
