// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BlueprintActionFilter.h"
#include "CoreMinimal.h"
#include "EdGraph/EdGraphSchema.h"
#include "JsonConvert/NodeHandler/LGNodeHandler.h"
#include "UObject/NoExportTypes.h"

class YELLOWPAINTEDITOR_API LGNodeHandler_Default : public FLGNodeHandler
{
public:
	// virtual TSharedPtr<FJsonObject> ToJson(const TObjectPtr<UEdGraphNode>) override;
	virtual bool ShouldExposeAction(FBlueprintActionFilter const& Filter, FBlueprintActionInfo& BlueprintAction) override;
};

REGISTER_LG_NODE_HANDLER(UK2Node, LGNodeHandler_Default);
