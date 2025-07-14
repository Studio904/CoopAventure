// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CoopAventure : ModuleRules
{
	public CoopAventure(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput" , "OnlineSubsystem", "OnlineSubsystemSteam" });
	}
}
