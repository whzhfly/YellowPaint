// Fill out your copyright notice in the Description page of Project Settings.


#include "Graph/YelloPaintSchema.h"
#include "LogicFlowNode.h"
/*#include "SCreatePinWindow.h"*/
#define LOCTEXT_NAMESPACE "YPGraphSchema"



static int32 NodeDistance = 60;

UEdGraphNode* FYellowPaintSchemaAction_NewNode::PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode)
{
	UEdGraphNode* ResultNode = NULL;

	// If there is a template, we actually use it
	if (NodeTemplate != NULL)
	{
		const FScopedTransaction Transaction(LOCTEXT("AddNode", "Add Node"));
		ParentGraph->Modify();
		if (FromPin)
		{
			FromPin->Modify();
		}

		NodeTemplate->SetFlags(RF_Transactional);

		// set outer to be the graph so it doesn't go away
		NodeTemplate->Rename(NULL, ParentGraph, REN_NonTransactional);
		ParentGraph->AddNode(NodeTemplate, true);

		NodeTemplate->CreateNewGuid();

		// For input pins, new node will generally overlap node being dragged off
		// Work out if we want to visually push away from connected node
		int32 XLocation = Location.X;
		if (FromPin && FromPin->Direction == EGPD_Input)
		{
			UEdGraphNode* PinNode = FromPin->GetOwningNode();
			const float XDelta = FMath::Abs(PinNode->NodePosX - Location.X);

			if (XDelta < NodeDistance)
			{
				// Set location to edge of current node minus the max move distance
				// to force node to push off from connect node enough to give selection handle
				XLocation = PinNode->NodePosX - NodeDistance;
			}
		}

		NodeTemplate->NodePosX = XLocation;
		NodeTemplate->NodePosY = Location.Y;
		NodeTemplate->SnapToGrid(16);
		UEdYellowPaintNode* EdNode = Cast<UEdYellowPaintNode>(NodeTemplate);
		if (PaletteBaseClass && EdNode)
		{
			EdNode->FlowNode = NewObject<ULogicFlowNode>(NodeTemplate, PaletteBaseClass, NAME_None);
		}
		// setup pins after placing node in correct spot, since pin sorting will happen as soon as link connection change occurs
		NodeTemplate->AllocateDefaultPins();
		if (FromPin)
		{
			NodeTemplate->AutowireNewNode(FromPin);
		}
		NodeTemplate->PostPlacedNewNode();

		ResultNode = NodeTemplate;
	}

	return ResultNode;
}

UEdGraphNode* FYellowPaintSchemaAction_NewNode::PerformAction(class UEdGraph* ParentGraph, TArray<UEdGraphPin*>& FromPins, const FVector2D Location, bool bSelectNewNode)
{
	UEdGraphNode* ResultNode = NULL;
	if (FromPins.Num() > 0)
	{
		ResultNode = PerformAction(ParentGraph, FromPins[0], Location);
		// Try autowiring the rest of the pins
		for (int32 Index = 1; Index < FromPins.Num(); ++Index)
		{
			ResultNode->AutowireNewNode(FromPins[Index]);
		}
	}
	else
	{
		ResultNode = PerformAction(ParentGraph, NULL, Location, bSelectNewNode);
	}
	return ResultNode;
}

void FYellowPaintSchemaAction_NewNode::AddReferencedObjects(FReferenceCollector& Collector)
{
	FEdGraphSchemaAction::AddReferencedObjects(Collector);

	// These don't get saved to disk, but we want to make sure the objects don't get GC'd while the action array is around
	Collector.AddReferencedObject(NodeTemplate);
}

UYelloPaintSchema::UYelloPaintSchema(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}


void UYelloPaintSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	/*const FName PinCategory = ContextMenuBuilder.FromPin ?
		ContextMenuBuilder.FromPin->PinType.PinCategory :
		UQuestEditorTypes::PinCategory_SingleNode;*/
	UObject* outer = ContextMenuBuilder.CurrentGraph->GetOuter();
	UBlueprint* BP = CastChecked<UBlueprint>(outer);


	{
		FString DisplayText = TEXT("可用节点:");
		FCategorizedGraphActionListBuilder ActionGraphActionBuilder(DisplayText);
		TArray<UClass*> ChildClasses;
		GetDerivedClasses(ULogicFlowNode::StaticClass(), ChildClasses);

		for (UClass* ChildClass : ChildClasses)
		{
			if (ChildClass->GetName().StartsWith(TEXT("SKEL_")) || ChildClass->GetName().StartsWith(TEXT("REINST_")))
			{
				continue; //过滤掉蓝图的SKEL和REINST类
			}
			UObject* DefaultObject = ChildClass->GetDefaultObject();
			ULogicFlowNode* defaultObj = CastChecked<ULogicFlowNode>(DefaultObject);
			//检查节点是否能在当前图使用
			/*if (defaultObj->GetPythonNodeTitle().EqualTo(FText::GetEmpty())) continue; //有些界面没名字*/
			/*if (bIsLevelQuest && defaultObj->QuestNodeType == EQuestNodeType::USE_IN_MAIN_SCENE) continue;*/
			/*if (!bIsLevelQuest && defaultObj->QuestNodeType == EQuestNodeType::USE_IN_EXPLORE_LEVEL) continue;*/
			TSharedPtr<FYellowPaintSchemaAction_NewNode> AddAction = UYelloPaintSchema::AddNewNodeAction(
				ActionGraphActionBuilder,
				FText::GetEmpty(),
				defaultObj->GetNodeTitle(),
				FText::GetEmpty());
			UEdYellowPaintNode* Node = NewObject<UEdYellowPaintNode>(ContextMenuBuilder.OwnerOfTemporaries, UEdYellowPaintNode::StaticClass());
			AddAction->NodeTemplate = Node;
			AddAction->PaletteBaseClass = ChildClass;
		}
		ContextMenuBuilder.Append(ActionGraphActionBuilder);
	}
}

void UYelloPaintSchema::GetContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const
{
	if (Context)
	{
		FToolMenuSection& Section = Menu->AddSection(NAME_None, FText::FromString(TEXT("任务流程控制")));

		UEdYellowPaintNode* EDFlowNode = Cast<UEdYellowPaintNode>(const_cast<UEdGraphNode*>(Context->Node.Get()));
		if (EDFlowNode) {
			{
				FText DisplayLabel = FText::FromString(TEXT("其他"));
				FText DisplayToolTips = FText::FromString(TEXT("其他"));
				Section.AddSubMenu("Add Component", DisplayLabel, DisplayToolTips,
					FNewMenuDelegate::CreateStatic(&UYelloPaintSchema::PopulateComponentMenu, EDFlowNode));
			}
			{
				FText DisplayLabel = FText::FromString(TEXT("自动生成引脚"));
				FText DisplayToolTips = FText::FromString(TEXT("针对某些特殊节点， 自动创建引脚"));
				Section.AddMenuEntry("Auto Add Pin", DisplayLabel, DisplayToolTips, FSlateIcon(),
					FUIAction(FExecuteAction::CreateStatic(&UYelloPaintSchema::AutoGenPinMenu, EDFlowNode))
				);
			}
		}
		Super::GetContextMenuActions(Menu, Context);
	}
}

void UYelloPaintSchema::PopulateComponentMenu(FMenuBuilder& MenuBuilder, UEdYellowPaintNode* EDFlowNode)
{
	/*MenuBuilder.AddMenuEntry(defaultObj->GetPythonNodeTitle(), defaultObj->GetPythonNodeTitle(), FSlateIcon(),
		FUIAction(
			FExecuteAction::CreateLambda([StepNode, ChildClass]() {
				UEdGraph* Graph = StepNode->GetGraph();
				UQuestGraphNode_ComponentBase* OpNode = NewObject<UQuestGraphNode_ComponentBase>(Graph, UQuestGraphNode_ComponentBase::StaticClass());
				OpNode->ComponentPythonBase = NewObject<UQuestComponentPythonBase>(OpNode, ChildClass, NAME_None);
				StepNode->AddSubNode(OpNode, Graph);
			}
	)));*/
}

void UYelloPaintSchema::AutoGenPinMenu(UEdYellowPaintNode* EDFlowNode)
{
	if (EDFlowNode)
	{
		EDFlowNode->ReconstructNode();
		TArray<UEdGraphPin*> AllPins = EDFlowNode->GetAllPins();
		for (auto pin : AllPins)
		{
			if (pin->Direction == EGPD_Output)
			{
				EDFlowNode->RemovePin(pin);
			}
		}
		EDFlowNode->AutoReFreshAPins();
	}
	EDFlowNode->GetGraph()->NotifyGraphChanged();
}

void UYelloPaintSchema::GetGraphDisplayInformation(const UEdGraph& Graph, FGraphDisplayInfo& DisplayInfo) const
{
	if (GetGraphType(&Graph) == GT_MAX)
	{
		DisplayInfo.PlainName = FText::FromString(Graph.GetName());
		DisplayInfo.DisplayName = DisplayInfo.PlainName;
		DisplayInfo.Tooltip = LOCTEXT("YYYYPPPP", "DisInfo.");
	}
	else
	{
		Super::GetGraphDisplayInformation(Graph, DisplayInfo);
	}
}

const FPinConnectionResponse UYelloPaintSchema::CanCreateConnection(const UEdGraphPin* NodeA, const UEdGraphPin* NodeB) const
{
	if (NodeA->Direction == NodeB->Direction) { // 两个pin方向不一样才能能连 out连out没有意义
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, LOCTEXT("PinErrorNotCompatible", "Connection not compatible"));
	}
	/*bool bAreBothStepNode = NodeA->GetOwningNode()->IsA<UQuestGraphNode_StepBase>() && NodeB->GetOwningNode()->IsA<UQuestGraphNode_StepBase>();
	if ( bAreBothStepNode) {
		return FPinConnectionResponse(CONNECT_RESPONSE_MAKE, LOCTEXT("PinConnect", "Connect nodes"));
	}*/
	if (NodeA->PinType == NodeB->PinType)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_MAKE, LOCTEXT("PinConnect", "Connect nodes"));
	}

	return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, LOCTEXT("PinErrorNotCompatible", "Connection not compatible"));
}


TSharedPtr<FYellowPaintSchemaAction_NewNode> UYelloPaintSchema::AddNewNodeAction(FGraphActionListBuilderBase& ContextMenuBuilder, const FText& Category, const FText& MenuDesc, const FText& Tooltip)
{
	TSharedPtr<FYellowPaintSchemaAction_NewNode> NewAction = TSharedPtr<FYellowPaintSchemaAction_NewNode>(new FYellowPaintSchemaAction_NewNode(Category, MenuDesc, Tooltip, 0));
	ContextMenuBuilder.AddAction(NewAction);
	return NewAction;
}


#undef LOCTEXT_NAMESPACE