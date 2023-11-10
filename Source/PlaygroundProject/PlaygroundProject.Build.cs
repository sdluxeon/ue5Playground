// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class PlaygroundProject : ModuleRules
{
    public PlaygroundProject(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Library"));
        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Library", "Camera"));
        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Library", "Characters"));
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "NavigationSystem", "AIModule", "Niagara", "EnhancedInput" });
    }
}
