// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class JoyWayTest : ModuleRules
{
	public JoyWayTest(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG" });

		PrivateDependencyModuleNames.AddRange(new string[] { "HeadMountedDisplay" });

		PublicIncludePaths.AddRange(new string[] { 
			"JoyWayTest/Public/Player",
			"JoyWayTest/Public/Grabbables",
			"JoyWayTest/Public/Grabbables/Weapon"
		});
	}
}
