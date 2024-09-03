// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class YellowPaintEditor : ModuleRules
{
    public YellowPaintEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        string engine_path = Path.GetFullPath(Target.RelativeEnginePath);
        string srcrt_path = engine_path + "Source/Editor/";
        PublicIncludePaths.Add(srcrt_path + "Kismet/Private");

        PrivateIncludePaths.AddRange(
            new string[] {
                // ... add other private include paths required here ...
                srcrt_path + "Kismet/Private"
            }
        );

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
			    // ... add other public dependencies that you statically link with here ...
			    "Core",
                "CoreUObject",
                "Engine",
                "BlueprintGraph",
                "SlateCore",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
			    // ... add private dependencies that you statically link with here ...	
                "EditorStyle",
                "InputCore",
                "Slate",
                "SlateCore",
                "UnrealEd",
                "BlueprintGraph",
                "Kismet",
                "KismetCompiler",
                "AssetTools",
                "Json",
                "JsonUtilities",
                // "EngineAssetDefinitions"
                "Projects",
                "PropertyEditor",
                "AnimGraph",
                "BlueprintGraph",
                "GraphEditor",
                "SubobjectEditor",
                "ContentBrowser",
                "ToolMenus",
                "YellowPaint",
                "AnimGraph",
                "KismetWidgets",
                "LevelEditor",
                "DeveloperSettings",
                "ContentBrowser",
            }
        );
    }

}
