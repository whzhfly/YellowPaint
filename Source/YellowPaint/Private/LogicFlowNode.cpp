// Fill out your copyright notice in the Description page of Project Settings.


#include "LogicFlowNode.h"
#include "LogicFlowDriverInstance.h"




void ULogicFlowNode::OnEnter_Implementation()
{
	
}


void ULogicFlowNode::OnExit_Implementation()
{
	
}

bool ULogicFlowNode::CheckAssetEnable_Implementation(EFlowDriverType DriveAsset)
{
	return true;
}


//============================================ Editor Meta ========================================

#if WITH_EDITOR

FText ULogicFlowNode::GetNodeTitle_Implementation()
{
	return this->GetClass()->GetDisplayNameText();
}

void ULogicFlowNode::CollectionAllocatePin_Implementation()
{
	AddAllocatePin(EEdGraphPinDirection::EGPD_Input, TEXT("exec"), TEXT("Out"));
	AddAllocatePin(EEdGraphPinDirection::EGPD_Output, TEXT("exec"), TEXT("In"));
}

void ULogicFlowNode::AddAllocatePin(EEdGraphPinDirection Dir, FName PinCategory, FName PinName)
{
	PinInfoStruct PinInfo(Dir, PinCategory, PinName);
	PinInfoArray.Add(PinInfo);
}

bool ULogicFlowNode::CanExtraAddPins_Implementation()
{
	return false;
}

void ULogicFlowNode::ExtraAddPins_Implementation()
{
}


void ULogicFlowNode::RefreshPins_Implementation()
{
}


void ULogicFlowNode::ReAutoGeneratePins()
{
	PinInfoArray.Empty();
	RefreshPins();
}

#endif