// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LogicFlowNode.h"
#include "YPBranchNode.generated.h"

/**
 * 
 */
UCLASS()
class YELLOWPAINT_API UYPBranchNode : public ULogicFlowNode
{
	GENERATED_BODY()

protected:
	void OnEnter_Implementation() override;
	
	void OnExit_Implementation() override;

public:

	void CollectionAllocatePin_Implementation() override;
	
	UPROPERTY(EditAnywhere, DisplayName="次数")
	int ExeCount;
};

UCLASS()
class YELLOWPAINT_API USequenceFlowNode : public ULogicFlowNode
{
	GENERATED_BODY()

/*protected:
	void OnEnter_Implementation() override;
	
	void OnExit_Implementation() override;*/

public:

	void CollectionAllocatePin_Implementation() override;
};

UCLASS()
class YELLOWPAINT_API UYAsyncNode : public ULogicFlowNode
{
	GENERATED_BODY()

protected:
	void OnEnter_Implementation() override;
	
	void OnExit_Implementation() override;

public:
	void CollectionAllocatePin_Implementation() override;
	
public:
	UPROPERTY(EditAnywhere, DisplayName="异步")
	bool HasBeenChecked;
};


UCLASS()
class YELLOWPAINT_API UFlowStartNode : public ULogicFlowNode
{
	GENERATED_BODY()

public:
	void CollectionAllocatePin_Implementation() override;

	
};

UCLASS()
class YELLOWPAINT_API UFlowEndNode : public ULogicFlowNode
{
	GENERATED_BODY()

public:
	void CollectionAllocatePin_Implementation() override;

};