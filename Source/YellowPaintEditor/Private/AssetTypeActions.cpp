// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetTypeActions.h"
#include "FYellowPaintGraphEditor.h"

#include "Kismet2/KismetEditorUtilities.h"
#include "BlueprintEditorTabs.h"
#include "Engine/SimpleConstructionScript.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "UYellowPaintGraph.h"
#include "YellowPaintEditorModule.h"


#define LOCTEXT_NAMESPACE "FAssetTypeActions_YellowPaintAsset"



UClass* FAssetTypeActions_YellowPaintAsset::GetSupportedClass() const
{
	return UYellowPaintGraph::StaticClass();
}

void FAssetTypeActions_YellowPaintAsset::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor /*= TSharedPtr<IToolkitHost>()*/)
{
	for (auto ObjIt = InObjects.CreateConstIterator(); ObjIt; ++ObjIt)
	{
		if (UYellowPaintGraph* Blueprint = Cast<UYellowPaintGraph>(*ObjIt))
		{
			bool bFoundExisting = false;

			FYellowPaintGraphEditor* ExistingInstance = nullptr;

			if (UAssetEditorSubsystem* AESubsystem = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>())
			{
				ExistingInstance = static_cast<FYellowPaintGraphEditor*>(AESubsystem->FindEditorForAsset(Blueprint, false));
			}

			if (!ExistingInstance)
			{		

				if (FYellowPaintEditorModule* EditorModule = FModuleManager::GetModulePtr<FYellowPaintEditorModule>("YellowPaintEditor"))
				{
					EditorModule->CreateEditor(Blueprint, EditWithinLevelEditor, SchemaClass);
				}
			}
		}
	}
}



#undef LOCTEXT_NAMESPACE