// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include "LogicFlowNode.h"
#include "EdYellowPaintNode.generated.h"

/**
 * 
 */
UCLASS()
class YELLOWPAINTEDITOR_API UEdYellowPaintNode : public UEdGraphNode
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<ULogicFlowNode> FlowNode;


	virtual FLinearColor GetGraphNodeColor() const { return FLinearColor(0.15f, 0.15f, 0.15f); };

	// UEdGraphNode
	//virtual void GetNodeContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const override;
	//virtual bool CanUserDeleteNode() const override;
	//virtual bool CanDuplicateNode() const override;
	//virtual TSharedPtr<SGraphNode> CreateVisualWidget() override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	//virtual FLinearColor GetNodeTitleColor() const override;
	//virtual FSlateIcon GetIconAndTint(FLinearColor& OutColor) const override;
	//virtual bool ShowPaletteIconOnNode() const override { return true; }
	//virtual FText GetTooltipText() const override;
	// --

	void AddUserOutput();

	bool CanUserAddInput() const;
	bool CanUserAddOutput() const;
	
	virtual FText GetFlowNodeTitleText() const;
	// todo
	virtual void AllocateDefaultPins();

	virtual void AutoReFreshAPins();
	
};
