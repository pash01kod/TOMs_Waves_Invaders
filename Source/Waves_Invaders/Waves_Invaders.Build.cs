// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Waves_Invaders : ModuleRules
{
	public Waves_Invaders(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
