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
	UPROPERTY(EditAnywhere, DisplayName="次数")
	int ExeCount;
};



UCLASS()
class YELLOWPAINT_API UYAsyncNode : public ULogicFlowNode
{
	GENERATED_BODY()

protected:
	void OnEnter_Implementation() override;
	
	void OnExit_Implementation() override;
	
public:
	UPROPERTY(EditAnywhere, DisplayName="异步")
	bool HasBeenChecked;
};
