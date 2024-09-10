// Fill out your copyright notice in the Description page of Project Settings.


#include "LogicFlowNode.h"




void ULogicFlowNode::OnEnter_Implementation()
{
	
}


void ULogicFlowNode::OnExit_Implementation()
{
	
}

FText ULogicFlowNode::GetNodeTitle_Implementation()
{
	return  FText::FromString(GetName());
}