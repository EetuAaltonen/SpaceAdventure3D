// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SpaceAdventure3D : ModuleRules
{
	public SpaceAdventure3D(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });
	}
}
