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


// =======================================
FText UEdYellowPaintNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return GetFlowNodeTitleText();
}

void UEdYellowPaintNode::AddUserOutput()
{
	if (FlowNode.Get())
	{
		CreatePin(EGPD_Output, TEXT("Narrator"), TEXT("11"));
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
		return true;
	}
	return false;
}
