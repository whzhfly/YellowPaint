// Fill out your copyright notice in the Description page of Project Settings.

#include "CommonNodes/YPBranchNode.h"




/*
void UYPBranchNode::OnEnter_Implementation()
{
	
}
	
void UYPBranchNode::OnExit_Implementation()
{
	
}

void UYPBranchNode::CollectionAllocatePin_Implementation()
{
	AddAllocatePin(EEdGraphPinDirection::EGPD_Input, TEXT("exec"), TEXT("In"));
	AddAllocatePin(EEdGraphPinDirection::EGPD_Input, TEXT("bool"), TEXT("Value"));

	AddAllocatePin(EEdGraphPinDirection::EGPD_Output, TEXT("exec"), TEXT("True"));
	AddAllocatePin(EEdGraphPinDirection::EGPD_Output, TEXT("exec"), TEXT("False"));
}

void USequenceFlowNode::CollectionAllocatePin_Implementation()
{
	AddAllocatePin(EEdGraphPinDirection::EGPD_Input, TEXT("exec"), TEXT("In"));

	AddAllocatePin(EEdGraphPinDirection::EGPD_Output, TEXT("exec"), TEXT("False"));
}

void UYAsyncNode::OnEnter_Implementation()
{
	
}
	
void UYAsyncNode::OnExit_Implementation()
{
	
}

void UYAsyncNode::CollectionAllocatePin_Implementation()
{
	AddAllocatePin(EEdGraphPinDirection::EGPD_Input, TEXT("exec"), TEXT("In"));
	AddAllocatePin(EEdGraphPinDirection::EGPD_Input, TEXT("int"), TEXT("Value"));
	AddAllocatePin(EEdGraphPinDirection::EGPD_Output, TEXT("exec"), TEXT("Out"));
}*/

void UFlowStartNode::CollectionAllocatePin_Implementation()
{
	AddAllocatePin(EEdGraphPinDirection::EGPD_Output, TEXT("exec"), TEXT("OUT"));
}

void UFlowEndNode::CollectionAllocatePin_Implementation()
{
	AddAllocatePin(EEdGraphPinDirection::EGPD_Input, TEXT("exec"), TEXT("IN"));
}

bool UFlowCheckNode::CheckAssetEnable_Implementation(EFlowDriverType DriveAsset)
{
	return false;
}

bool UFlowAsyncNode::CheckAssetEnable_Implementation(EFlowDriverType DriveAsset)
{
	return false;
}

bool UFlowEndNode::CheckAssetEnable_Implementation(EFlowDriverType DriveAsset)
{
	return false;
}