// Copyright Epic Games, Inc. All Rights Reserved.

#include "Style/YellowPaintEditorStyle.h"
#include "YellowPaint.h"
#include "Framework/Application/SlateApplication.h"
#include "Styling/SlateStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Styling/CoreStyle.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

/*#define RootToContentDir Style->RootToContentDir*/



TSharedPtr<FSlateStyleSet> FYellowPaintEditorStyle::StyleInstance = nullptr;

FString FYellowPaintEditorStyle::RootToContentDir(const ANSICHAR* RelativePath, const TCHAR* Extension)
{
	static FString ContentDir = IPluginManager::Get().FindPlugin(TEXT("YellowPaint"))->GetContentDir();
	return (ContentDir / RelativePath) + Extension;
}

void FYellowPaintEditorStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance.Get());
	}

	{
		StyleInstance->Set(FName(TEXT("ClassThumbnail.NarrativeTask")), new IMAGE_BRUSH("NarrativeIcon64x64", FVector2D(64, 64)));
		StyleInstance->Set(FName(TEXT("QuestEditor.FlowControl")), new IMAGE_BRUSH("CaseQuestIcon64x64", FVector2D(64, 64)));
		StyleInstance->Set(FName(TEXT("QuestEditor.ComponentDelete")), new IMAGE_BRUSH("QuestIcon16x16", FVector2D(64, 64)));
	}
}

void FYellowPaintEditorStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance.Get());
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FYellowPaintEditorStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("YellowPaintStyle"));
	return StyleSetName;
}


TSharedRef<FSlateStyleSet> FYellowPaintEditorStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("YellowPaintStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("YellowPaint")->GetBaseDir() / TEXT("Resources"));
	return Style;
}

void FYellowPaintEditorStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FYellowPaintEditorStyle::Get()
{
	return *StyleInstance;
}
