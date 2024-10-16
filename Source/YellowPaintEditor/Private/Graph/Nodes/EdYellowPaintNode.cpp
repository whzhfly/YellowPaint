// Fill out your copyright notice in the Description page of Project Settings.


#include "Graph/Nodes/EdYellowPaintNode.h"




FText UEdYellowPaintNode::GetFlowNodeTitleText() const
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
}


void UEdYellowPaintNode::AutoReFreshAPins()
{
	if (FlowNode)
	{
		FlowNode->ReAutoGeneratePins();
		for (auto pinInfo: FlowNode->PinInfoArray) {
			CreatePin(pinInfo.Dir, pinInfo.PinCategory, pinInfo.PinName);
		}
	}
}

// =======================================
FText UEdYellowPaintNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return GetFlowNodeTitleText();
}

void UEdYellowPaintNode::AddUserOutput()
{
	if (FlowNode.Get())
	{
		FlowNode->PinInfoArray.Empty();
		FlowNode.Get()->ExtraAddPins();
		for (auto pinInfo: FlowNode->PinInfoArray) {
			CreatePin(pinInfo.Dir, pinInfo.PinCategory, pinInfo.PinName);
		}
	}
	GetGraph()->NotifyGraphChanged();
}


bool UEdYellowPaintNode::CanUserAddInput() const
{
	if (FlowNode.Get())
	{
		return true;
	}
	return false;
}

bool UEdYellowPaintNode::CanUserAddOutput() const
{
	if (FlowNode.Get())
	{
		return FlowNode.Get()->CanExtraAddPins();;
	}
	return false;
}
