// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateStyle.h"
#include "Styling/ISlateStyle.h"

class FYellowPaintEditorStyle
{
public:

	static void Initialize();

	static void Shutdown();

	/** reloads textures used by slate renderer */
	static void ReloadTextures();

	/** @return The Slate style set for the Shooter game */
	static const ISlateStyle& Get();

	static FName GetStyleSetName();

private:

	static TSharedRef<FSlateStyleSet> Create();
	
	static FString RootToContentDir(const ANSICHAR* RelativePath, const TCHAR* Extension);

private:

	static TSharedPtr<FSlateStyleSet> StyleInstance;
};