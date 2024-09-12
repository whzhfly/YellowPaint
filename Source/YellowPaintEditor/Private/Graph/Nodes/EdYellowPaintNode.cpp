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
	
}
