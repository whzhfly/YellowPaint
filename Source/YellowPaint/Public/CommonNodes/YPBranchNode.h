// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LogicFlowNode.h"
#include "YPBranchNode.generated.h"

/**
 * 
 */


UCLASS(DisplayName="异步任务")
class YELLOWPAINT_API UFlowAsyncNode : public ULogicFlowNode
{
	GENERATED_BODY()
};


UCLASS(DisplayName="开始")
class YELLOWPAINT_API UFlowStartNode : public ULogicFlowNode
{
	GENERATED_BODY()

public:
	void CollectionAllocatePin_Implementation() override;
	
};

UCLASS(DisplayName="结束")
class YELLOWPAINT_API UFlowEndNode : public ULogicFlowNode
{
	GENERATED_BODY()

public:
	void CollectionAllocatePin_Implementation() override;

};

UCLASS(DisplayName="检测")
class YELLOWPAINT_API UFlowCheckNode : public ULogicFlowNode
{
	GENERATED_BODY()

};