// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include "LogicFlowNode.h"
#include "EdYellowPaintNode.generated.h"

/**
 * 
 */
UCLASS()
class YELLOWPAINTEDITOR_API UEdYellowPaintNode : public UEdGraphNode
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<ULogicFlowNode> FlowNode;


	virtual FLinearColor GetGraphNodeColor() const { return FLinearColor(0.15f, 0.15f, 0.15f); };

	virtual FText GetNodeTitleText() const;
	// todo
	virtual void AllocateDefaultPins();
	
};
