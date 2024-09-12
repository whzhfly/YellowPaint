// Fill out your copyright notice in the Description page of Project Settings.

#include "CommonNodes/YPBranchNode.h"




void UYPBranchNode::OnEnter_Implementation()
{
	
}
	
void UYPBranchNode::OnExit_Implementation()
{
	
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
}

void UFlowStartNode::CollectionAllocatePin_Implementation()
{
	AddAllocatePin(EEdGraphPinDirection::EGPD_Output, TEXT("exec"), TEXT("Out"));
}

void UFlowEndNode::CollectionAllocatePin_Implementation()
{
	AddAllocatePin(EEdGraphPinDirection::EGPD_Input, TEXT("exec"), TEXT("In"));
}