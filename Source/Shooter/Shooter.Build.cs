// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Shooter : ModuleRules
{
    public Shooter(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.Default;
        PrivatePCHHeaderFile = "Shooter.h";

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine", 
                "InputCore",
                "HeadMountedDisplay",
                "UMG"
            });

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "Networking",
                "Sockets",
            }
        );
    }
}