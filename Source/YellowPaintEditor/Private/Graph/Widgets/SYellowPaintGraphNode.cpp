// Fill out your copyright notice in the Description page of Project Settings.


#include "Graph/Widgets//SYellowPaintGraphNode.h"
#include "Style/YellowPaintEditorStyle.h"
#include "SLevelOfDetailBranchNode.h"
#include "GraphEditorSettings.h"
#include "TutorialMetaData.h"
#include "SCommentBubble.h"
#include "ScopedTransaction.h"
#include "SGraphNode.h"
#include "SGraphPanel.h"
#include "SGraphPin.h"
#include "SLevelOfDetailBranchNode.h"
#include "SNodePanel.h"
#include "Styling/SlateColor.h"
#include "TutorialMetaData.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/SOverlay.h"
#include "Widgets/SToolTip.h"
#include "Widgets/Text/SInlineEditableTextBlock.h"

#define LOCTEXT_NAMESPACE "SYellowPaintGraphNode"


const FLinearColor SYellowPaintGraphNode::UnselectedNodeTint = FLinearColor(1.0f, 1.0f, 1.0f, 0.5f);
const FLinearColor SYellowPaintGraphNode::ConfigBoxColor = FLinearColor(0.04f, 0.04f, 0.04f, 1.0f);


void SYellowPaintGraphNode::Construct(const FArguments& InArgs, UEdYellowPaintNode* InNode)
{
	PointNode = InNode;
	GraphNode = InNode;
	UpdateGraphNode();
}

FMargin SYellowPaintGraphNode::ComputeSubNodeChildIndentPaddingMargin() const
{
	return FMargin();
	/*if (!IsValid(PointNode) || !PointNode->IsSubNode())
	{
		return FMargin();
	}*/

	/*
	const UEdYellowPaintNode* CurrentAncestor = PointNode->GetParentNode();

	// Compute the parent depth, so it can be used to determine the indent level for this subnode
	int32 ParentDepth = 0;
	while (IsValid(CurrentAncestor))
	{
		++ParentDepth;

		CurrentAncestor = CurrentAncestor->GetParentNode();
	}

	constexpr float VerticalDefaultPadding = 2.0f;
	constexpr float HorizontalDefaultPadding = 2.0f;
	constexpr float IndentedHorizontalPadding = 6.0f;
	constexpr float RightPadding = HorizontalDefaultPadding;
	float LeftPadding = HorizontalDefaultPadding;

	if (ParentDepth > 0)
	{
		// Increase the padding by the parent depth for this node
		LeftPadding = IndentedHorizontalPadding * ParentDepth;
	}
	else
	{
		LeftPadding = 0.0f;
	}

	return
		FMargin(
			LeftPadding,
			VerticalDefaultPadding,
			RightPadding,
			VerticalDefaultPadding);*/
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

	TSharedPtr<SVerticalBox> MainVerticalBox;
	SetupErrorReporting();

	const TSharedPtr<SNodeTitle> NodeTitle = SNew(SNodeTitle, GraphNode);

	// Get node icon
	IconColor = FLinearColor::White;
	const FSlateBrush* IconBrush = nullptr;
	if (GraphNode && GraphNode->ShowPaletteIconOnNode())
	{
		IconBrush = GraphNode->GetIconAndTint(IconColor).GetOptionalIcon();
	}

	// Compute the SubNode padding indent based on the parentage depth for this node
	const FMargin NodePadding = ComputeSubNodeChildIndentPaddingMargin();

	const TSharedRef<SOverlay> DefaultTitleAreaWidget = SNew(SOverlay)
	+ SOverlay::Slot()
	.HAlign(HAlign_Fill)
	.VAlign(VAlign_Center)
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.HAlign(HAlign_Fill)
		[
			SNew(SBorder)
			/*.BorderImage(FFlowEditorStyle::GetBrush("Flow.Node.Title"))*/
			.BorderImage(FYellowPaintEditorStyle::Get().GetBrush("Flow.Node.Title")) // todo
			// The extra margin on the right is for making the color spill stretch well past the node title
			.Padding(FMargin(10, 5, 30, 3))
			.BorderBackgroundColor(this, &SYellowPaintGraphNode::GetBorderBackgroundColor)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
					.VAlign(VAlign_Top)
					.Padding(FMargin(0.f, 0.f, 4.f, 0.f))
					.AutoWidth()
					[
						SNew(SImage)
						.Image(IconBrush)
						.ColorAndOpacity(this, &SYellowPaintGraphNode::GetNodeTitleIconColor)
					]
				+ SHorizontalBox::Slot()
					[
						SNew(SVerticalBox)
						+ SVerticalBox::Slot()
							.AutoHeight()
							[
								CreateTitleWidget(NodeTitle)
							]
						+ SVerticalBox::Slot()
							.AutoHeight()
							[
								NodeTitle.ToSharedRef()
							]
					]
			]
		]
	];

	SetDefaultTitleAreaWidget(DefaultTitleAreaWidget);

	const TSharedRef<SWidget> TitleAreaWidget = 
		SNew(SLevelOfDetailBranchNode)
			.UseLowDetailSlot(this, &SYellowPaintGraphNode::UseLowDetailNodeTitles)
			.LowDetail()
			[
				SNew(SBorder)
					/*.BorderImage(FFlowEditorStyle::GetBrush("Flow.Node.Title"))*/
					.BorderImage(FYellowPaintEditorStyle::Get().GetBrush("Flow.Node.Title"))
					.Padding(FMargin(75.0f, 22.0f)) // Saving enough space for a 'typical' title so the transition isn't quite so abrupt
					.BorderBackgroundColor(this, &SGraphNode::GetNodeTitleColor)
			]
			.HighDetail()
			[
				DefaultTitleAreaWidget
			];

	// Setup a meta tag for this node
	FGraphNodeMetaData TagMeta(TEXT("FlowGraphNode"));
	PopulateMetaTag(&TagMeta);

	this->ContentScale.Bind(this, &SGraphNode::GetContentScale);

	const TSharedPtr<SVerticalBox> InnerVerticalBox = SNew(SVerticalBox)
		+ SVerticalBox::Slot()
			.AutoHeight()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Top)
			.Padding(Settings->GetNonPinNodeBodyPadding())
			[
				TitleAreaWidget
			]
		+ SVerticalBox::Slot()
			.AutoHeight()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Top)
			[
				CreateNodeContentArea()
			];


	const TSharedPtr<SWidget> EnabledStateWidget = GetEnabledStateWidget();
	if (EnabledStateWidget.IsValid())
	{
		InnerVerticalBox->AddSlot()
			.AutoHeight()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Top)
			.Padding(FMargin(2, 0))
			[
				EnabledStateWidget.ToSharedRef()
			];
	}

	InnerVerticalBox->AddSlot()
		.AutoHeight()
		.Padding(Settings->GetNonPinNodeBodyPadding())
		[
			ErrorReporting->AsWidget()
		];

	this->GetOrAddSlot(ENodeZone::Center)
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SAssignNew(MainVerticalBox, SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(FMargin(NodePadding.Left, 0.0f, NodePadding.Right, 0.0f))
			[
				SNew(SOverlay)
					.AddMetaData<FGraphNodeMetaData>(TagMeta)
					+ SOverlay::Slot()
						.Padding(Settings->GetNonPinNodeBodyPadding())
						[
							SNew(SImage)
							.Image(GetNodeBodyBrush())
							.ColorAndOpacity(this, &SYellowPaintGraphNode::GetNodeBodyColor)
						]
					+ SOverlay::Slot()
						[
							InnerVerticalBox.ToSharedRef()
						]
			]
		];

	if (GraphNode && GraphNode->SupportsCommentBubble())
	{
		// Create comment bubble
		TSharedPtr<SCommentBubble> CommentBubble;
		const FSlateColor CommentColor = GetDefault<UGraphEditorSettings>()->DefaultCommentNodeTitleColor;

		SAssignNew(CommentBubble, SCommentBubble)
			.GraphNode(GraphNode)
			.Text(this, &SGraphNode::GetNodeComment)
			.OnTextCommitted(this, &SGraphNode::OnCommentTextCommitted)
			.OnToggled(this, &SGraphNode::OnCommentBubbleToggled)
			.ColorAndOpacity(CommentColor)
			.AllowPinning(true)
			.EnableTitleBarBubble(true)
			.EnableBubbleCtrls(true)
			.GraphLOD(this, &SGraphNode::GetCurrentLOD)
			.IsGraphNodeHovered(this, &SGraphNode::IsHovered);

		GetOrAddSlot(ENodeZone::TopCenter)
			.SlotOffset(TAttribute<FVector2D>(CommentBubble.Get(), &SCommentBubble::GetOffset))
			.SlotSize(TAttribute<FVector2D>(CommentBubble.Get(), &SCommentBubble::GetSize))
			.AllowScaling(TAttribute<bool>(CommentBubble.Get(), &SCommentBubble::IsScalingAllowed))
			.VAlign(VAlign_Top)
			[
				CommentBubble.ToSharedRef()
			];
	}

	CreateBelowWidgetControls(MainVerticalBox);
	CreatePinWidgets();
	CreateInputSideAddButton(LeftNodeBox);
	CreateOutputSideAddButton(RightNodeBox);
	CreateBelowPinControls(InnerVerticalBox);
	CreateAdvancedViewArrow(InnerVerticalBox);

	
	/// quest----------------------other
	/*FSlateRenderTransform QuantityRender;
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
			]#1#
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
	}*/
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


FSlateColor SYellowPaintGraphNode::GetBorderBackgroundColor() const
{
	return SGraphNode::GetNodeTitleColor();
}


void SYellowPaintGraphNode::AddPinButton(TSharedPtr<SVerticalBox> OutputBox, const TSharedRef<SWidget> ButtonContent, const EEdGraphPinDirection Direction, const FString DocumentationExcerpt, const TSharedPtr<SToolTip> CustomTooltip)
{
	const FText PinTooltipText = (Direction == EEdGraphPinDirection::EGPD_Input) ? LOCTEXT("FlowNodeAddPinButton_InputTooltip", "Adds an input pin") : LOCTEXT("FlowNodeAddPinButton_OutputTooltip", "Adds an output pin");
	TSharedPtr<SToolTip> Tooltip;

	/*if (CustomTooltip.IsValid())
	{
		Tooltip = CustomTooltip;
	}
	else if (!DocumentationExcerpt.IsEmpty())
	{
		Tooltip = IDocumentation::Get()->CreateToolTip(PinTooltipText, nullptr, GraphNode->GetDocumentationLink(), DocumentationExcerpt);
	}*/

	/*const TSharedRef<SButton> AddPinButton = SNew(SButton)
		.ContentPadding(0.0f)
		.ButtonStyle(FAppStyle::Get(), "NoBorder")
		.OnClicked(this, &SFlowGraphNode::OnAddFlowPin, Direction)
		.IsEnabled(this, &SFlowGraphNode::IsNodeEditable)
		.ToolTipText(PinTooltipText)
		.ToolTip(Tooltip)
		.Visibility(this, &SFlowGraphNode::IsAddPinButtonVisible)
		[
			ButtonContent
		];

	AddPinButton->SetCursor(EMouseCursor::Hand);

	FMargin AddPinPadding = (Direction == EEdGraphPinDirection::EGPD_Input) ? Settings->GetInputPinPadding() : Settings->GetOutputPinPadding();
	AddPinPadding.Top += 6.0f;

	OutputBox->AddSlot()
		.AutoHeight()
		.VAlign(VAlign_Center)
		.Padding(AddPinPadding)
		[
			AddPinButton
		];*/
}

FReply SYellowPaintGraphNode::OnAddFlowPin(const EEdGraphPinDirection Direction)
{
	switch (Direction)
	{
	case EGPD_Input:
		/*FlowGraphNode->AddUserInput();*/
		break;
	case EGPD_Output:
		/*FlowGraphNode->AddUserOutput();*/
		break;
	default:
		break;
	}

	return FReply::Handled();
}

const FSlateBrush* SYellowPaintGraphNode::GetNodeBodyBrush() const
{
	return FYellowPaintEditorStyle::Get().GetBrush("Flow.Node.Body");
}

FSlateColor SYellowPaintGraphNode::GetNodeTitleColor() const
{
	FLinearColor ReturnTitleColor = GraphNode->IsDeprecated() ? FLinearColor::Red : GetNodeObj()->GetNodeTitleColor();

	ReturnTitleColor.A = FadeCurve.GetLerp();

	/*if (FlowGraphNode->GetSignalMode() == EFlowSignalMode::Enabled)
	{
		ReturnTitleColor.A = FadeCurve.GetLerp();
	}
	else
	{
		ReturnTitleColor *= FLinearColor(0.5f, 0.5f, 0.5f, 0.4f);
	}

	if (!IsFlowGraphNodeSelected(FlowGraphNode) && FlowGraphNode->IsSubNode())
	{
		ReturnTitleColor *= UnselectedNodeTint;
	}*/

	return ReturnTitleColor;
}

FSlateColor SYellowPaintGraphNode::GetNodeBodyColor() const
{
	FLinearColor ReturnBodyColor = GraphNode->GetNodeBodyTintColor();
	ReturnBodyColor *= FLinearColor(1.0f, 1.0f, 1.0f, 0.5f); 
	/*if (FlowGraphNode->GetSignalMode() != EFlowSignalMode::Enabled)
	{
		ReturnBodyColor *= FLinearColor(1.0f, 1.0f, 1.0f, 0.5f); 
	}
	else if (!IsFlowGraphNodeSelected(FlowGraphNode) && FlowGraphNode->IsSubNode())
	{
		ReturnBodyColor *= UnselectedNodeTint;
	}*/

	return ReturnBodyColor;
}

FSlateColor SYellowPaintGraphNode::GetNodeTitleIconColor() const
{
	FLinearColor ReturnIconColor = IconColor;
	ReturnIconColor *= FLinearColor(1.0f, 1.0f, 1.0f, 0.3f);
	ReturnIconColor *= UnselectedNodeTint;
	/*if (FlowGraphNode->GetSignalMode() != EFlowSignalMode::Enabled)
	{
		ReturnIconColor *= FLinearColor(1.0f, 1.0f, 1.0f, 0.3f); 
	}
	else if (!IsFlowGraphNodeSelected(FlowGraphNode) && FlowGraphNode->IsSubNode())
	{
		ReturnIconColor *= UnselectedNodeTint;
	}*/

	return ReturnIconColor;
}

FLinearColor SYellowPaintGraphNode::GetNodeTitleTextColor() const
{
	FLinearColor ReturnTextColor = FLinearColor::White;
	ReturnTextColor *= FLinearColor(1.0f, 1.0f, 1.0f, 0.3f); 
	/*if (FlowGraphNode->GetSignalMode() != EFlowSignalMode::Enabled)
	{
		ReturnTextColor *= FLinearColor(1.0f, 1.0f, 1.0f, 0.3f); 
	}
	else if (!IsFlowGraphNodeSelected(FlowGraphNode) && FlowGraphNode->IsSubNode())
	{
		ReturnTextColor *= UnselectedNodeTint;
	}*/

	return ReturnTextColor;
}


/*
TSharedPtr<SWidget> SYellowPaintGraphNode::GetEnabledStateWidget() const
{
	if (FlowGraphNode->IsSubNode())
	{
		// SubNodes don't get enabled/disabled on their own,
		//  they follow the enabled/disabled setting of their owning flow node

		return TSharedPtr<SWidget>();
	}

	if (FlowGraphNode->GetSignalMode() != EFlowSignalMode::Enabled && !GraphNode->IsAutomaticallyPlacedGhostNode())
	{
		const bool bPassThrough = FlowGraphNode->GetSignalMode() == EFlowSignalMode::PassThrough;
		const FText StatusMessage = bPassThrough ? LOCTEXT("PassThrough", "Pass Through") : LOCTEXT("DisabledNode", "Disabled");
		const FText StatusMessageTooltip = bPassThrough ?
			LOCTEXT("PassThroughTooltip", "This node won't execute internal logic, but it will trigger all connected outputs") :
			LOCTEXT("DisabledNodeTooltip", "This node is disabled and will not be executed");

		return SNew(SBorder)
			.BorderImage(FAppStyle::GetBrush(bPassThrough ? "Graph.Node.DevelopmentBanner" : "Graph.Node.DisabledBanner"))
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(STextBlock)
				.Text(StatusMessage)
				.ToolTipText(StatusMessageTooltip)
				.Justification(ETextJustify::Center)
				.ColorAndOpacity(FLinearColor::White)
				.ShadowOffset(FVector2D::UnitVector)
				.Visibility(EVisibility::Visible)
			];
	}

	return TSharedPtr<SWidget>();
}
*/

#undef LOCTEXT_NAMESPACE