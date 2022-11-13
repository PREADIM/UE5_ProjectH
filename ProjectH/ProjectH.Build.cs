// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProjectH : ModuleRules
{
	public ProjectH(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "GameSetting" ,"InputCore", "AIModule", "GameplayTasks", "NavigationSystem", "Slate", "SlateCore", "UMG", "ImageWrapper", "LevelSequence", "MovieScene" });


		PrivateDependencyModuleNames.AddRange(new string[] { "GameSetting" });
	}
}
