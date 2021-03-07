// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class dpProject : ModuleRules
{
	public dpProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "ProceduralMeshComponent" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

	}
}
