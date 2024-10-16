// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphSchema.h"
#include "EdGraphSchema_K2.h"
#include "Graph/Nodes/EdYellowPaintNode.h"
#include "YelloPaintSchema.generated.h"

/**
 * 
 */


USTRUCT()
struct YELLOWPAINTEDITOR_API FYellowPaintSchemaAction_NewNode : public FEdGraphSchemaAction
{
	GENERATED_USTRUCT_BODY();

	/** Template of node we want to create */
	/*UPROPERTY()
	class UQuestGraphNode* NodeTemplate;*/

	UPROPERTY()
	class UEdYellowPaintNode* NodeTemplate;

	UPROPERTY()
	TObjectPtr<UClass> PaletteBaseClass;

	FYellowPaintSchemaAction_NewNode()
		: FEdGraphSchemaAction()
		, NodeTemplate(nullptr)
	{}

	FYellowPaintSchemaAction_NewNode(FText InNodeCategory, FText InMenuDesc, FText InToolTip, const int32 InGrouping)
		: FEdGraphSchemaAction(MoveTemp(InNodeCategory), MoveTemp(InMenuDesc), MoveTemp(InToolTip), InGrouping)
		, NodeTemplate(nullptr)
	{}

	//~ Begin FEdGraphSchemaAction Interface
	virtual UEdGraphNode* PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode = true) override;
	virtual UEdGraphNode* PerformAction(class UEdGraph* ParentGraph, TArray<UEdGraphPin*>& FromPins, const FVector2D Location, bool bSelectNewNode = true) override;
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
	//~ End FEdGraphSchemaAction Interface

	template <typename NodeType>
	static NodeType* SpawnNodeFromTemplate(class UEdGraph* ParentGraph, NodeType* InTemplateNode, const FVector2D Location)
	{
		FYellowPaintSchemaAction_NewNode Action;
		Action.NodeTemplate = InTemplateNode;

		return Cast<NodeType>(Action.PerformAction(ParentGraph, nullptr, Location));
	}

};

UCLASS(MinimalAPI)
class UYelloPaintSchema : public UEdGraphSchema
{
	GENERATED_BODY()

public:
	UYelloPaintSchema(const FObjectInitializer& ObjectInitializer);


	static void PopulateComponentMenu(FMenuBuilder& MenuBuilder, class UEdYellowPaintNode* EDFlowNode);
	static void AutoGenPinMenu(class UEdYellowPaintNode* EDFlowNode);

	virtual void GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const override;
	virtual void GetContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const override;

	virtual void GetGraphDisplayInformation(const UEdGraph& Graph, FGraphDisplayInfo& DisplayInfo) const override;


	virtual const FPinConnectionResponse CanCreateConnection(const UEdGraphPin* NodeA, const UEdGraphPin* NodeB) const override;
protected:
	static TSharedPtr<FYellowPaintSchemaAction_NewNode> AddNewNodeAction(FGraphActionListBuilderBase& ContextMenuBuilder, const FText& Category, const FText& MenuDesc, const FText& Tooltip);
};
