// Fill out your copyright notice in the Description page of Project Settings.


#include "Graph/UEdYellowPaintGraph.h"
#include "EdGraph/EdGraphPin.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "GraphEditAction.h"
#include "AssetRegistry/AssetRegistryModule.h"


UEdYellowPaintGraph::UEdYellowPaintGraph(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}


void UEdYellowPaintGraph::NotifyGraphChanged()
{
	Super::NotifyGraphChanged();
	UBlueprint* Blueprint = FBlueprintEditorUtils::FindBlueprintForGraph(this);
	if (Blueprint)
	{
		FBlueprintEditorUtils::MarkBlueprintAsModified(Blueprint);
	}
	
}

void UEdYellowPaintGraph::NotifyGraphChanged(const FEdGraphEditAction& Action)
{
	Super::NotifyGraphChanged(Action);
	UBlueprint* Blueprint = FBlueprintEditorUtils::FindBlueprintForGraph(this);
	if (Blueprint)
	{
		FBlueprintEditorUtils::MarkBlueprintAsModified(Blueprint);
	}

	if (Action.Action == EEdGraphActionType::GRAPHACTION_RemoveNode)
	{
		for (auto& DeletedNode : Action.Nodes)
		{
			/*//如果删除节点是StepBase 且有对应的扩展蓝图, 就把对应的扩展蓝图也移除
			UQuestGraphNode_StepBase* StepNode = const_cast<UQuestGraphNode_StepBase*>(Cast<UQuestGraphNode_StepBase>(DeletedNode));
			if (!StepNode || StepNode->NodeRelatedBlueprintPath.IsEmpty()) continue;

			TArray<FAssetData> Assets;
			UObject* FoundObject = LoadObject<UObject>(NULL, *StepNode->NodeRelatedBlueprintPath);
			Assets.Add(FoundObject);
			ObjectTools::DeleteAssets(Assets, false);*/
		}
	}
}

void UEdYellowPaintGraph::PinRewired(UEdGraphNode* Node, UEdGraphPin* Pin)
{
}

void UEdYellowPaintGraph::NodeAdded(UEdGraphNode* AddedNode)
{
}