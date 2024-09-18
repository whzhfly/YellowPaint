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
#include "Styling/SlateStyleRegistry.h"

/*#define RootToContentDir Style->RootToContentDir*/


#define BORDER_BRUSH( RelativePath, ... ) FSlateBorderBrush( StyleInstance->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define BOX_BRUSH( RelativePath, ... ) FSlateBoxBrush( StyleInstance->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define IMAGE_BRUSH( RelativePath, ... ) FSlateImageBrush( StyleInstance->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define IMAGE_BRUSH_SVG( RelativePath, ... ) FSlateVectorImageBrush(StyleInstance->RootToContentDir(RelativePath, TEXT(".svg")), __VA_ARGS__)

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
	const FVector2D Icon16(16.0f, 16.0f);
	const FVector2D Icon20(20.0f, 20.0f);
	const FVector2D Icon30(30.0f, 30.0f);
	const FVector2D Icon40(40.0f, 40.0f);
	const FVector2D Icon64(64.0f, 64.0f);

	{

		// Flow assets
		StyleInstance->SetContentRoot(IPluginManager::Get().FindPlugin("YellowPaint")->GetBaseDir() / TEXT("Resources"));
		
		// StyleInstance->SetContentRoot(IPluginManager::Get().FindPlugin(TEXT("Flow"))->GetBaseDir() / TEXT("Resources"));

		StyleInstance->Set("ClassIcon.LogicFlowAsset", new IMAGE_BRUSH("Icons/FlowAsset_16x", Icon16));
		StyleInstance->Set("ClassThumbnail.LogicFlowAsset", new IMAGE_BRUSH("Icons/FlowAsset_64x", Icon64));
		
		StyleInstance->Set("Flow.Node.Title", new BOX_BRUSH("Icons/FlowNode_Title", FMargin(8.0f/64.0f, 0, 0, 0)));
		StyleInstance->Set("Flow.Node.Body", new BOX_BRUSH("Icons/FlowNode_Body", FMargin(16.f/64.f)));
		StyleInstance->Set("Flow.Node.ActiveShadow", new BOX_BRUSH("Icons/FlowNode_Shadow_Active", FMargin(18.0f/64.0f)));
		StyleInstance->Set("Flow.Node.WasActiveShadow", new BOX_BRUSH("Icons/FlowNode_Shadow_WasActive", FMargin(18.0f/64.0f)));


		StyleInstance->Set("Flow.Node.ClassIcon", new IMAGE_BRUSH(TEXT("ActionIcon64x64"), Icon64));

		StyleInstance->Set("ClassIcon.FlowAsset", new IMAGE_BRUSH(TEXT("Icons/FlowAsset_16x"), Icon16));
		
		/*StyleInstance->Set(FName(TEXT("ClassThumbnail.NarrativeTask")), new IMAGE_BRUSH("ActionIcon64x64", Icon64));*/
		StyleInstance->Set(FName(TEXT("QuestEditor.FlowControl")), new IMAGE_BRUSH("CaseQuestIcon64x64", Icon64));
		StyleInstance->Set(FName(TEXT("QuestEditor.ComponentDelete")), new IMAGE_BRUSH("QuestIcon16x16", Icon64));
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
