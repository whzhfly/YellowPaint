// Fill out your copyright notice in the Description page of Project Settings.


#include "PaletteNode.h"




void UPaletteNode::OnEnter_Implementation()
{
	
}


void UPaletteNode::OnExit_Implementation()
{
	
}

FText UPaletteNode::GetNodeTitle_Implementation()
{
	return  FText::FromString(GetName());
}