// Fill out your copyright notice in the Description page of Project Settings.


#include "Graph/Nodes/EdYellowPaintNode.h"




FText UEdYellowPaintNode::GetNodeTitleText() const
{
	if (FlowNode.Get())
	{
		return  FlowNode.Get()->GetNodeTitle();
	}
	return FText::FromString("PaintNode");
}

void UEdYellowPaintNode::AllocateDefaultPins()
{
	if (FlowNode)
	{
		FlowNode->CollectionAllocatePin();
		for (auto pinInfo: FlowNode->PinInfoArray) {
			CreatePin(pinInfo.Dir, pinInfo.PinCategory, pinInfo.PinName);
		}
	}
	
	/*CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, TEXT("Out"));
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, TEXT("In"));

	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Int, TEXT("Int"));
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Boolean, TEXT("bool"));


	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Int, TEXT("Int"));
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Boolean, TEXT("bool"));


	UEdGraphNode::FCreatePinParams InputPinParams;
	InputPinParams.ContainerType = EPinContainerType::None;
	InputPinParams.ValueTerminalType = FEdGraphTerminalType();
	InputPinParams.ValueTerminalType .TerminalCategory = UEdGraphSchema_K2::PC_Int;
	/*PinParams.ValueTerminalType = ProtoPinType.PinValueType;#1#
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Wildcard, TEXT("Wild"), InputPinParams);

	UEdGraphNode::FCreatePinParams PinParams;
	PinParams.ContainerType = EPinContainerType::None;
	PinParams.ValueTerminalType = FEdGraphTerminalType();
	PinParams.ValueTerminalType .TerminalCategory = UEdGraphSchema_K2::PC_Boolean;
	/*PinParams.ValueTerminalType = ProtoPinType.PinValueType;#1#
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Wildcard, TEXT("Wild"), PinParams);*/
}