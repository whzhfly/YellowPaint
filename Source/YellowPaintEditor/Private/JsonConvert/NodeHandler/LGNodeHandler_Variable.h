// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BlueprintActionFilter.h"
#include "CoreMinimal.h"
#include "EdGraph/EdGraphSchema.h"
#include "JsonConvert/NodeHandler/LGNodeHandler.h"
#include "UObject/NoExportTypes.h"
#include "K2Node_Variable.h"

class YELLOWPAINTEDITOR_API FLGNodeHandler_Variable : public FLGNodeHandler
{
public:
	virtual TSharedPtr<FJsonObject> ToJson(const TObjectPtr<UEdGraphNode>) override;
	virtual bool ShouldExposeAction(FBlueprintActionFilter const& Filter, FBlueprintActionInfo& BlueprintAction) override;
};

REGISTER_LG_NODE_HANDLER(UK2Node_Variable, FLGNodeHandler_Variable);
