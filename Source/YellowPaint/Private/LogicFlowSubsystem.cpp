// Fill out your copyright notice in the Description page of Project Settings.


#include "LogicFlowSubsystem.h"




ULogicFlowDriverInstance*  ULogicFlowSubsystem::CreateLogicFLow(ULogicFlowAsset* FlowAsset)
{
	if (FlowAsset)
	{
		return CreateLogicFlowDriverInstance(FlowAsset->FlowInstance);
	}
	return nullptr;
}

void ULogicFlowSubsystem::RunLogicFlow(ULogicFlowDriverInstance* FlowDriver)
{
	if (FlowDriver)
	{
		FlowDriver->StartFlow();
	}
}


ULogicFlowDriverInstance* ULogicFlowSubsystem::CreateLogicFlowDriverInstance(ULogicFlowDriverInstance* FlowDriver)
{
	ULogicFlowDriverInstance* NewInstance = NewObject<ULogicFlowDriverInstance>(this, ULogicFlowDriverInstance::StaticClass());
	NewInstance->DeepCopyFormAnother(FlowDriver);
	return NewInstance;
}