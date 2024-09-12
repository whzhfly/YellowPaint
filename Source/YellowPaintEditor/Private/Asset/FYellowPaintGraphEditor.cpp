// Fill out your copyright notice in the Description page of Project Settings.


#include "Asset/FYellowPaintGraphEditor.h"

#include "LogicFlowAsset.h"
#include "EditorCommand/YellowPaintGraphEditorCommands.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ToolMenu.h"
#include "Toolkits/AssetEditorToolkit.h"
#include "Toolkits/AssetEditorToolkitMenuContext.h"
#include "ParseGenerate/YPGraphInfoGenerator.h"
#include "Graph/YelloPaintSchema.h"
#include "Graph/UEdYellowPaintGraph.h"
#include "Graph/Nodes/EdYellowPaintNode.h"
#include "Kismet2/BlueprintEditorUtils.h"

#define LOCTEXT_NAMESPACE "YpGraphEditor"
DECLARE_LOG_CATEGORY_CLASS(YpLogGraphEditor, Log, All);

void FYellowPaintGraphEditor::InitBlueprintEditorExt(
	const EToolkitMode::Type Mode, const TSharedPtr<class IToolkitHost>& InitToolkitHost, const TArray<class UBlueprint*>& InBlueprints,
	bool bShouldOpenInDefaultsMode, UClass* InSchemaClass
)
{
	FBlueprintEditorModule* BEModule = FModuleManager::GetModulePtr<FBlueprintEditorModule>("Kismet");
	/*auto BlueprintEditorTabSpawnerHandle = BEModule->OnRegisterTabsForEditor().AddRaw(this, &FLogicGraphEditor::RegisterBlueprintEditorTab);*/

	FBlueprintEditor::InitBlueprintEditor(Mode, InitToolkitHost, InBlueprints, bShouldOpenInDefaultsMode);
	/*BEModule->OnRegisterTabsForEditor().Remove(BlueprintEditorTabSpawnerHandle);*/

	CreateDefaultCommands();
	
	TArray<UEdGraph*> Graphs;
	class UBlueprint* Blueprint = InBlueprints[0];
	Blueprint->GetAllGraphs(Graphs);
	int Count = 0;

	UEdGraph* YpGraph = nullptr;
	FName LogicFlowGraphName = FName("LogicFlow");

	for (UEdGraph* Graph : Graphs)
	{
		/*if (Graph->GetFName() == UEdGraphSchema_K2::GN_EventGraph)
		{
			Count += 1;
			if (Graph->Schema != InSchemaClass)
			{
				Graph->Schema = InSchemaClass;
			}
		}*/
		if (Graph->GetFName() == LogicFlowGraphName)
		{
			Count += 1;
			YpGraph = Graph;
		}
	}

	if(YpGraph == nullptr)
	{
		YpGraph =
				FBlueprintEditorUtils::CreateNewGraph(Blueprint, LogicFlowGraphName, UEdYellowPaintGraph::StaticClass(), UYelloPaintSchema::StaticClass());
		YpGraph->bAllowDeletion =
			false;	//@TODO: Really, just want to make sure we never drop below 1, not that you cannot delete any particular one!
		YpGraph->bAllowRenaming = false;

		FBlueprintEditorUtils::AddUbergraphPage(Blueprint, YpGraph);
	}
	if (YpGraph)
	{
		this->OpenDocument(YpGraph, FDocumentTracker::ForceOpenNewDocument);
	}
	/*
	if (Count == 0)
	{
		// FKismetEditorUtilities::CreateDefaultEventGraphs(Blueprint);
		{
			UEdGraph* Ubergraph =
				FBlueprintEditorUtils::CreateNewGraph(Blueprint, UEdGraphSchema_K2::GN_EventGraph, UEdGraph::StaticClass(), InSchemaClass);
			Ubergraph->bAllowDeletion =
				false;	//@TODO: Really, just want to make sure we never drop below 1, not that you cannot delete any particular one!
			Ubergraph->bAllowRenaming = false;

			FBlueprintEditorUtils::AddUbergraphPage(Blueprint, Ubergraph);

			Blueprint->LastEditedDocuments.AddUnique(Ubergraph);
		}
	}
	*/

	/*for (UEdGraph* Graph : Graphs)
	{
		if (Graph->GetFName() == UEdGraphSchema_K2::FN_UserConstructionScript)
		{
			this->CloseDocumentTab(Graph);
			break;
		}
	}*/
	/*Graphs.Empty();
	Blueprint->GetAllGraphs(Graphs);*/
	/*for (UEdGraph* Graph : Graphs)
	{
		if (Graph->GetFName() == UEdGraphSchema_K2::GN_EventGraph)
		{
			this->OpenDocument(Graph, FDocumentTracker::ForceOpenNewDocument);
			break;
		}
	}*/
}

UToolMenu* FYellowPaintGraphEditor::RegisterModeToolbarIfUnregistered(const FName InModeName)
{
	UToolMenu* TempToolbar = FBlueprintEditor::RegisterModeToolbarIfUnregistered(InModeName);
	if (TempToolbar)
	{
		/*GetToolbarBuilder()->AddCompileToolbar(TempToolbar);
		GetToolbarBuilder()->AddScriptingToolbar(TempToolbar);
		// GetToolbarBuilder()->AddBlueprintGlobalOptionsToolbar(TempToolbar, CanAccessComponentsMode());
		GetToolbarBuilder()->AddBlueprintGlobalOptionsToolbar(TempToolbar, false);
		// GetToolbarBuilder()->AddDebuggingToolbar(TempToolbar);*/
		AddBlueprintLGOtherToolBar(TempToolbar);
	}
	return TempToolbar;
}

void FYellowPaintGraphEditor::AddBlueprintLGOtherToolBar(UToolMenu* InMenu)
{
	FToolMenuSection& Section = InMenu->FindOrAddSection("Logic");
	/*Section.InsertPosition = FToolMenuInsert("Settings", EToolMenuInsertType::After);*/

	Section.AddDynamicEntry(
		"ExportJSON",
		FNewToolMenuSectionDelegate::CreateLambda(
			[](FToolMenuSection& InSection)
			/*{
				/*UBlueprintEditorToolMenuContext* Context = InSection.FindContext<UBlueprintEditorToolMenuContext>();#1#
				/*if (Context && Context->BlueprintEditor.IsValid() && Context->GetBlueprintObj())
				{
					TSharedPtr<FLogicGraphEditor> LGEditor = StaticCastSharedPtr<FLogicGraphEditor>(Context->BlueprintEditor.Pin());
					if (LGEditor)
					{
						const FLogicGraphEditorCommands& Commands = FLogicGraphEditorCommands::Get();
						InSection.AddEntry(FToolMenuEntry::InitToolBarButton(
							Commands.ExportJSONCommand,
							TAttribute<FText>(),
							TAttribute<FText>(),
							FSlateIcon(FAppStyle::GetAppStyleSetName(), "AssetEditor.SaveAsset.Greyscale")
						));
					}
				}#1#
				const FYellowPaintGraphEditorCommands& Commands = FYellowPaintGraphEditorCommands::Get();
				InSection.AddEntry(FToolMenuEntry::InitToolBarButton(
							Commands.ExportJSONCommand,
							TAttribute<FText>(),
							TAttribute<FText>(),
							FSlateIcon(FAppStyle::GetAppStyleSetName(), "AssetEditor.SaveAsset.Greyscale")
						));
			}*/
			{
				UAssetEditorToolkitMenuContext* MenuContext = InSection.FindContext<UAssetEditorToolkitMenuContext>();
				if (MenuContext && MenuContext->Toolkit.IsValid())
				{
					TSharedPtr<FAssetEditorToolkit> Toolkit = MenuContext->Toolkit.Pin();
					if (Toolkit->IsActuallyAnAsset() && Toolkit->GetObjectsCurrentlyBeingEdited()->Num() == 1)
					{
						for (const UObject* EditedObject : *Toolkit->GetObjectsCurrentlyBeingEdited())
						{
							/*EditedObject->context*/
							/*if (IsClassHeaderViewSupported(EditedObject->GetClass()))
							{*/
								const FYellowPaintGraphEditorCommands& Commands = FYellowPaintGraphEditorCommands::Get();
								InSection.AddEntry(FToolMenuEntry::InitToolBarButton(
										Commands.ExportJSONCommand,
										TAttribute<FText>(),
										TAttribute<FText>(),
										FSlateIcon(FAppStyle::GetAppStyleSetName(), "AssetEditor.SaveAsset.Greyscale")
									));
							/*}*/
						}
					}
				}
			}
			
		)
	);
}

void FYellowPaintGraphEditor::CreateDefaultCommands()
{
	// Call Super
	FBlueprintEditor::CreateDefaultCommands();

	FYellowPaintGraphEditorCommands::Register();

	TWeakPtr<FBlueprintEditor> WeakThisPtr = SharedThis(this);
	ToolkitCommands->MapAction(
		FYellowPaintGraphEditorCommands::Get().ExportJSONCommand,
		FExecuteAction::CreateSP(this, &FYellowPaintGraphEditor::ExportJSON),
		FCanExecuteAction::CreateSP(this, &FYellowPaintGraphEditor::IsCompilingEnabled)
	);
	ToolkitCommands->MapAction(
	FYellowPaintGraphEditorCommands::Get().ShowDetails,
	FExecuteAction::CreateSP(this, &FYellowPaintGraphEditor::ExportJSON),
	FCanExecuteAction::CreateSP(this, &FYellowPaintGraphEditor::IsCompilingEnabled)
	);
}

void FYellowPaintGraphEditor::ExportJSON()
{
	UBlueprint* BlueprintObj = GetBlueprintObj();
	if (!BlueprintObj) return;
	FString BPPath = UKismetSystemLibrary::GetSystemPath(BlueprintObj);
	TArray<FAssetData> SelectData;
	SelectData.Add(BlueprintObj);
	YPGraphInfoGenerator::Generate(SelectData);
	UE_LOG(YpLogGraphEditor, Log, TEXT("Export %s To JSON "), *BPPath);
}

void FYellowPaintGraphEditor::OnSelectedNodesChangedImpl(const TSet<class UObject*>& NewSelection)
{
	/**
	***/
	UBlueprint* BlueprintObj = GetBlueprintObj();
	ULogicFlowAsset* FlowAsset = Cast<ULogicFlowAsset>(BlueprintObj);
	if (FlowAsset != nullptr)
	{
		TSet<class UObject*> PropertyModifiedSelection;
		PropertyModifiedSelection.Add(FlowAsset->FlowInstance);
		/*SelectedNodeGUID = GraphNode->NodeGuid.ToString();*/
		FBlueprintEditor::OnSelectedNodesChangedImpl(PropertyModifiedSelection);
	}
	
	if (NewSelection.Num() == 1)
	{
		for (auto& Obj : NewSelection)
		{
			//Want to edit the underlying quest object, not the graph node
			UEdYellowPaintNode* GraphNode = Cast<UEdYellowPaintNode>(Obj);
			if (GraphNode && GraphNode->FlowNode)
			{

				TSet<class UObject*> ModifiedSelection;
				ModifiedSelection.Add(GraphNode->FlowNode);
				/*SelectedNodeGUID = GraphNode->NodeGuid.ToString();*/
				FBlueprintEditor::OnSelectedNodesChangedImpl(ModifiedSelection);
				return;
			}
			/*UQuestGraphNode_ComponentBase* ComponentNode = Cast<UQuestGraphNode_ComponentBase>(Obj);
			if (ComponentNode && ComponentNode->ComponentPythonBase)
			{

				TSet<class UObject*> ModifiedSelection;
				ModifiedSelection.Add(ComponentNode->ComponentPythonBase);

				auto TimerManager = GEditor->GetTimerManager();
				FTimerHandle TimerHandle;
				/**
				* 这里有个贼操蛋的bug, 如果是第一次从编辑器外直接点编辑器的组件, 会导致
				* SKismetInspector::ShowDetailsForObjects先show了组件的python base 再show节点
				* 但是node不是这个顺序 node先show节点再show的python base 就能正常显示
				* 只能加个timer跳过这个时序bug 不过临时fix一下也没发现别的啥问题就是了

				*#1#
				TimerManager->SetTimer(TimerHandle, [this, ModifiedSelection]()
					{
						FBlueprintEditor::OnSelectedNodesChangedImpl(ModifiedSelection);
					}, 0.1f, false);
				return;
			}*/
		}
	}


	FBlueprintEditor::OnSelectedNodesChangedImpl(NewSelection);
}


FGraphAppearanceInfo FYellowPaintGraphEditor::GetGraphAppearance(UEdGraph* InGraph) const
{
	FGraphAppearanceInfo BPInfo = FBlueprintEditor::GetGraphAppearance(InGraph);
	// BPInfo.CornerText =  FText::FromString(TEXT("🍌逻辑编辑器🤖🍇🍉🍍🍓"));
	BPInfo.CornerText =  FText::FromString(TEXT("🤖Flow"));
	/*UQuestGraph* MaybeQuestGraph = Cast<UQuestGraph>(InGraph);*/
	/*UObject* outer = MaybeQuestGraph->GetOuter();
	UBlueprint* BP = CastChecked<UBlueprint>(outer);*/
	/*if (BP->ParentClass == ULevelQuestLogicBase::StaticClass()) {
		BPInfo.CornerText = FText::FromString(TEXT("关卡任务编辑器🤖"));
	}
	else if (BP->ParentClass == UCaseQuestLogicBase::StaticClass()) {
		BPInfo.CornerText = FText::FromString(TEXT("CASE编辑器📒"));
	}
	else if (BP->ParentClass == UQuestLogicBase::StaticClass()) {
		BPInfo.CornerText = FText::FromString(TEXT("任务编辑器🕹"));
	}*/
	return BPInfo;
}

#undef LOCTEXT_NAMESPACE