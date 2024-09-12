// Fill out your copyright notice in the Description page of Project Settings.


#include "Graph/Widgets//SYellowPaintGraphNode.h"
#include "Style/YellowPaintEditorStyle.h"




void SYellowPaintGraphNode::Construct(const FArguments& InArgs, UEdYellowPaintNode* InNode)
{
	PointNode = InNode;
	GraphNode = InNode;
	UpdateGraphNode();
}

void SYellowPaintGraphNode::UpdateGraphNode()
{

	InputPins.Empty();
	OutputPins.Empty();

	// Reset variables that are going to be exposed, in case we are refreshing an already setup node.
	RightNodeBox.Reset();
	LeftNodeBox.Reset();
	/*ComponentWidgets.Reset();*/

	//	     ______________________
	//	    |      TITLE AREA      |
	//	    +-------+------+-------+
	//	    | (>) L |      | R (>) |
	//	    | (>) E |      | I (>) |
	//	    | (>) F |      | G (>) |
	//	    | (>) T |      | H (>) |
	//	    |       |      | T (>) |
	//	    |_______|______|_______|
	//
	

	FSlateRenderTransform QuantityRender;
	QuantityRender.SetTranslation(FVector2D(0.f, -30.f));

	FSlateRenderTransform CurrentStateIconRender;
	CurrentStateIconRender.SetTranslation(FVector2D(0.f, -96.f));
	
	UEdYellowPaintNode* PaintNode = Cast<UEdYellowPaintNode>(PointNode);

	if (PaintNode)
	{
		auto SizeBox = SNew(SBox);
		SizeBox->SetMinDesiredWidth(250.f);
		SizeBox->SetMinDesiredHeight(60.f);
		this->GetOrAddSlot(ENodeZone::Center)
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SBorder)
				.BorderImage(FAppStyle::GetBrush("BTEditor.Graph.BTNode.Body"))
			.BorderBackgroundColor(this, &SYellowPaintGraphNode::GetBorderColor)
			.OnMouseButtonDown(this, &SYellowPaintGraphNode::OnMouseDown)
			[
				SNew(SOverlay)
				+ SOverlay::Slot()
			[
				SizeBox
			]
		+ SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.Padding(5.f)
			[
				SNew(SVerticalBox)
				+SVerticalBox::Slot()
				.AutoHeight()[
					SNew(SBorder)
						.BorderImage(FAppStyle::GetBrush("BTEditor.Graph.BTNode.Body"))
						.BorderBackgroundColor(FLinearColor::Black)
						.Visibility(this, &SYellowPaintGraphNode::GetNodeDebugTextVis)
						[
							SNew(SOverlay)
					+ SOverlay::Slot()
						.HAlign(HAlign_Fill)
						.VAlign(VAlign_Fill)
						.Padding(5.f)
						[
							SNew(SVerticalBox)
							+ SVerticalBox::Slot()
						.AutoHeight()[
							SNew(STextBlock)
								.Text(this, &SYellowPaintGraphNode::GetNodeDebugText)
								.Visibility(this, &SYellowPaintGraphNode::GetNodeDebugTextVis)
								.Justification(ETextJustify::Center)
								.TextStyle(FAppStyle::Get(), TEXT("PhysicsAssetEditor.Tools.Font"))
								.WrapTextAt(200.f)
						]
				]]]
				/*+ SVerticalBox::Slot()
			.AutoHeight()
			[
				ComponentBox.ToSharedRef()
			]*/
		+ SVerticalBox::Slot()
			.AutoHeight()[
				SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.VAlign(VAlign_Center)
					[
						SNew(SImage)
						.Image(SYellowPaintGraphNode::GetFlowControlIcon())
					.Visibility(this, &SYellowPaintGraphNode::GetNodeFlowControlVis)
					]

				+ SHorizontalBox::Slot()
					.VAlign(VAlign_Center)
					.HAlign(HAlign_Left)
					.AutoWidth()
					[
						SNew(SBox)
						.MinDesiredHeight(10.f)
					[
						SAssignNew(LeftNodeBox, SVerticalBox)
					]
					]
				+ SHorizontalBox::Slot()
					.AutoWidth()
					.VAlign(VAlign_Center)
					[
						SNew(SImage)
						.Image(SYellowPaintGraphNode::GetNameIcon())
					.Visibility(this, &SYellowPaintGraphNode::GetNodeIDVis)
					]
				+ SHorizontalBox::Slot()
					.VAlign(VAlign_Fill)
					.HAlign(HAlign_Fill)
					.FillWidth(1.f)
					[
						SNew(SVerticalBox)
						+ SVerticalBox::Slot()
					.VAlign(VAlign_Top)
					.HAlign(HAlign_Fill)
					.AutoHeight()
					[
						SNew(STextBlock)
						.Text(this, &SYellowPaintGraphNode::GetNodeTitleText)
					.ColorAndOpacity(this, &SYellowPaintGraphNode::GetNodeTitleColor)
					.Justification(ETextJustify::Center)
					.Clipping(EWidgetClipping::Inherit)
					.TextStyle(FAppStyle::Get(), TEXT("PhysicsAssetEditor.Tools.Font"))
					]
				+ SVerticalBox::Slot()
					.VAlign(VAlign_Bottom)
					.HAlign(HAlign_Fill)
					.AutoHeight()
					[
						SNew(STextBlock)
						.Text(this, &SYellowPaintGraphNode::GetNodeID)
					.Visibility(this, &SYellowPaintGraphNode::GetNodeIDVis)
					.Justification(ETextJustify::Center)
					.WrapTextAt(200.f)
					]
				+ SVerticalBox::Slot()
					.VAlign(VAlign_Bottom)
					.HAlign(HAlign_Fill)
					.AutoHeight()
					[
						SNew(STextBlock)
						.Text(this, &SYellowPaintGraphNode::GetNodeText)
					.Justification(ETextJustify::Center)
					.WrapTextAt(200.f)
					]
					+ SVerticalBox::Slot()
					.VAlign(VAlign_Bottom)
					.HAlign(HAlign_Fill)
					.AutoHeight()
					[
						SNew(STextBlock)
							.Text(this, &SYellowPaintGraphNode::GetConditionText)
							.Justification(ETextJustify::Center)
							.WrapTextAt(200.f)
					]
				+ SVerticalBox::Slot()
					.VAlign(VAlign_Center)
					.HAlign(HAlign_Fill)
					.FillHeight(1.f)
					[
						SNew(STextBlock)
						.Text(this, &SYellowPaintGraphNode::GetEventsText)
					.Visibility(this, &SYellowPaintGraphNode::GetEventsVis)
					.Justification(ETextJustify::Center)
					.AutoWrapText(true)
					.TextStyle(FAppStyle::Get(), TEXT("PhysicsAssetEditor.Tools.Font"))
					.ColorAndOpacity(this, &SYellowPaintGraphNode::GetEventsColor)
					]
					]
				+ SHorizontalBox::Slot()
					.VAlign(VAlign_Center)
					.HAlign(HAlign_Right)
					.AutoWidth()
					[
						SNew(SBox)
						.MinDesiredHeight(10.f)
					[
						SAssignNew(RightNodeBox, SVerticalBox)
					]
					]
			]
			]
			]
			];

		CreatePinWidgets();
	}
}


FText SYellowPaintGraphNode::GetNodeTitleText() const
{
	UEdYellowPaintNode* Node = CastChecked<UEdYellowPaintNode>(PointNode);
	return Node->GetNodeTitleText();
}

const FSlateBrush* SYellowPaintGraphNode::GetNameIcon() const
{
	return FYellowPaintEditorStyle::Get().GetBrush(TEXT("ClassThumbnail.NarrativeTask"));
}
const FSlateBrush* SYellowPaintGraphNode::GetFlowControlIcon() const
{
	return FYellowPaintEditorStyle::Get().GetBrush(TEXT("QuestEditor.FlowControl"));
}
const FSlateBrush* SYellowPaintGraphNode::GetComponentDeleteIcon() const
{
	return FYellowPaintEditorStyle::Get().GetBrush(TEXT("QuestEditor.ComponentDelete"));
}

FText SYellowPaintGraphNode::GetNodeText() const
{
	return FText::FromString("GetNodeText");
}

EVisibility SYellowPaintGraphNode::GetNodeFlowControlVis() const
{
	return  EVisibility::Collapsed;
}

FText SYellowPaintGraphNode::GetConditionText() const
{
	return FText::FromString("GetConditionText");
}

EVisibility SYellowPaintGraphNode::GetNodeIDVis() const
{
	return GetNodeID().IsEmpty() ? EVisibility::Collapsed : EVisibility::Visible;
}

EVisibility SYellowPaintGraphNode::GetEventsVis() const
{
	return GetEventsText().IsEmpty() ? EVisibility::Collapsed : EVisibility::Visible;
}


EVisibility SYellowPaintGraphNode::GetNodeDebugTextVis() const
{
	return  GetNodeDebugText().IsEmpty() ? EVisibility::Collapsed : EVisibility::Visible;
}

FText SYellowPaintGraphNode::GetNodeID() const
{
	/*if (!GraphNode->IsA<UQuestGraphNode_StepBase>()) return FText();
	UQuestGraphNode_StepBase* Node = CastChecked<UQuestGraphNode_StepBase>(GraphNode);
	if (Node && Node->QuestPythonNode) {
		return FText::AsNumber(Node->QuestPythonNode->QuestID);
	}*/
	return FText();
	/*return FText::AsNumber(Node->QuestID);*/
}

FText SYellowPaintGraphNode::GetEventsText() const
{
	FString EvtText = "";
	return FText::FromString(EvtText);
}

FSlateColor SYellowPaintGraphNode::GetBorderColor() const
{
	return FLinearColor::Gray;
}

FReply SYellowPaintGraphNode::OnMouseDown(const FGeometry& SenderGeometry, const FPointerEvent& MouseEvent)
{
	return FReply::Unhandled();
}

FSlateColor SYellowPaintGraphNode::GetEventsColor() const
{
	return  FSlateColor(FColor::White);
}

FText SYellowPaintGraphNode::GetNodeDebugText() const
{
	FString CountStr = "GetNodeDebugText";
	FText DebugMsg = FText::FromString(CountStr);
	return DebugMsg;
}
