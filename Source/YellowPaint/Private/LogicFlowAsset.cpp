// Fill out your copyright notice in the Description page of Project Settings.


#include "LogicFlowAsset.h"


ULogicFlowAsset::ULogicFlowAsset(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	FlowInstance = CreateDefaultSubobject<ULogicFlowDriverInstance>(TEXT("DefaultFlowInstance"));
	FlowInstance->SetFlags(RF_Transactional | RF_ArchetypeObject);
}


void ULogicFlowAsset::PostLoad()
{
	Super::PostLoad();
	FlowInstance->SetFlags(RF_Transactional | RF_ArchetypeObject);
}
