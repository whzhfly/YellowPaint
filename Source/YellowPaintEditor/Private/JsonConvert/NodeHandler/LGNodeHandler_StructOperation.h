// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BlueprintActionFilter.h"
#include "CoreMinimal.h"
#include "EdGraph/EdGraphSchema.h"
#include "JsonConvert/NodeHandler/LGNodeHandler.h"
#include "UObject/NoExportTypes.h"
#include "K2Node_StructOperation.h"

#define REGISTER_LG_ALLOWED_STRUCT(...) \
	static auto LG_CONCAT(_reg_lg_allowed_struct_, __COUNTER__) = FLGNodeHandler_StructOperation::RegisterAllowedStruct({__VA_ARGS__})

class YELLOWPAINTEDITOR_API FLGNodeHandler_StructOperation : public FLGNodeHandler
{
public:
	static TSet<FString> AllowedStruct;
	static bool RegisterAllowedStruct(std::initializer_list<FString>);
public:
	virtual TSharedPtr<FJsonObject> ToJson(const TObjectPtr<UEdGraphNode>) override;
	virtual bool ShouldExposeAction(FBlueprintActionFilter const& Filter, FBlueprintActionInfo& BlueprintAction) override;
};

REGISTER_LG_NODE_HANDLER(UK2Node_StructOperation, FLGNodeHandler_StructOperation);
