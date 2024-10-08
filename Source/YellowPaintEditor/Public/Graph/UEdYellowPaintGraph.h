// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "EdGraph/EdGraph.h"
#include "UEdYellowPaintGraph.generated.h"

/**
 * 
 */
UCLASS()
class YELLOWPAINTEDITOR_API UEdYellowPaintGraph : public UEdGraph
{
	GENERATED_BODY()

public:
	UEdYellowPaintGraph(const FObjectInitializer& ObjectInitializer);


	virtual void NotifyGraphChanged() override;
	virtual void NotifyGraphChanged(const FEdGraphEditAction& Action) override;

	void PinRewired(UEdGraphNode* Node, UEdGraphPin* Pin);
	void NodeAdded(UEdGraphNode* AddedNode);

};
