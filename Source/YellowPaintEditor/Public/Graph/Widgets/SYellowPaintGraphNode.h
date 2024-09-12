// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SGraphNode.h"
#include "Graph/Nodes/EdYellowPaintNode.h"

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


	// SNodePanel::SNode
	/*virtual void GetNodeInfoPopups(FNodeInfoContext* Context, TArray<FGraphInformationPopupInfo>& Popups) const override;
	virtual const FSlateBrush* GetShadowBrush(bool bSelected) const override;
	virtual void GetOverlayBrushes(bool bSelected, const FVector2D WidgetSize, TArray<FOverlayBrushInfo>& Brushes) const override;*/
	// --

	TObjectPtr<UEdYellowPaintNode> PointNode;
	virtual void UpdateGraphNode() override;
	// other style info
	FText GetNodeTitleText() const;
	
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

public:

	// SGraphNode
	/*virtual void GetPinBrush(const bool bLeftSide, const float WidgetWidth, const int32 PinIndex, const FFlowPinTrait& Breakpoint, TArray<FOverlayBrushInfo>& Brushes) const;*/

	/*virtual FText GetTitle() const;*/
	/*virtual FText GetDescription() const;*/
	/*virtual EVisibility GetDescriptionVisibility() const;*/

	/*virtual FText GetPreviewCornerText() const;*/
	virtual const FSlateBrush* GetNameIcon() const;

	virtual FSlateColor GetBorderBackgroundColor() const;

	/*virtual FSlateColor GetConfigBoxBackgroundColor() const;*/

	
	FMargin ComputeSubNodeChildIndentPaddingMargin() const;
	
	// Variant of SGraphNode::AddPinButtonContent
	virtual void AddPinButton(TSharedPtr<SVerticalBox> OutputBox, TSharedRef<SWidget> ButtonContent, const EEdGraphPinDirection Direction, FString DocumentationExcerpt = FString(), TSharedPtr<SToolTip> CustomTooltip = nullptr);

	// Variant of SGraphNode::OnAddPin
	virtual FReply OnAddFlowPin(const EEdGraphPinDirection Direction);
};