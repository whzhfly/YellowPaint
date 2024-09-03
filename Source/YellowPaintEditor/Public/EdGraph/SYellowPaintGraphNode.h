// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SGraphNode.h"
#include "Schemal/EdYellowPaintNode.h"

/**
 * 
 */
class UEdYellowPaintNode;

/**
 * 
 */
class YELLOWPAINTEDITOR_API SYellowPaintGraphNode : public SGraphNode
{
public:
	SLATE_BEGIN_ARGS(SYellowPaintGraphNode)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs, UEdYellowPaintNode* InNode);
	virtual void UpdateGraphNode() override;


	TObjectPtr<UEdYellowPaintNode> PointNode;


	// other style info
	FText GetNodeTitleText() const;
	
	const FSlateBrush* GetNameIcon() const;
	const FSlateBrush* GetFlowControlIcon() const;
	const FSlateBrush* GetComponentDeleteIcon() const;
	
	FText GetNodeText() const;
	EVisibility GetNodeFlowControlVis() const;
	FText GetConditionText() const;
	EVisibility GetNodeIDVis() const;
	EVisibility GetEventsVis() const;
	FText GetNodeID() const;
	FText GetEventsText() const;
	FSlateColor GetBorderColor() const;
	FReply OnMouseDown(const FGeometry& SenderGeometry, const FPointerEvent& MouseEvent);
	FSlateColor GetEventsColor() const;
	EVisibility GetNodeDebugTextVis() const;
	FText GetNodeDebugText() const;
};