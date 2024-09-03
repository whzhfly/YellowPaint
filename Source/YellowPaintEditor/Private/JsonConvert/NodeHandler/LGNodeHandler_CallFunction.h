// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BlueprintActionFilter.h"
#include "CoreMinimal.h"
#include "EdGraph/EdGraphSchema.h"
#include "K2Node_CallFunction.h"
#include "JsonConvert/NodeHandler/LGNodeHandler.h"
#include "UObject/NoExportTypes.h"

#define REGISTER_LG_ALLOWED_STATIC_FUNCTION(...) \
	static auto LG_CONCAT(_reg_lg_allowed_static_functino_, __COUNTER__) = FLGNodeHandler_CallFunction::RegisterAllowedStaticFunction({__VA_ARGS__})

#define REGISTER_LG_ALLOWED_STATIC_FUNCTION_OF_CLASS(OwnerClass, ...)      \
	static auto LG_CONCAT(_reg_lg_allowed_static_functino_, __COUNTER__) = \
		FLGNodeHandler_CallFunction::RegisterAllowedStaticFunctionOfClass(OwnerClass, {__VA_ARGS__})

class YELLOWPAINTEDITOR_API FLGNodeHandler_CallFunction : public FLGNodeHandler
{
public:
	static TSet<FString> AllowedStaticFunctions;
	static bool RegisterAllowedStaticFunction(std::initializer_list<FString> functions);
	static bool RegisterAllowedStaticFunctionOfClass(FString OwnerClass, std::initializer_list<FString> functions);

public:
	virtual TSharedPtr<FJsonObject> ToJson(const TObjectPtr<UEdGraphNode>) override;
	/*virtual bool ShouldExposeAction(FBlueprintActionFilter const& Filter, FBlueprintActionInfo& BlueprintAction) override;*/
};

REGISTER_LG_NODE_HANDLER(UK2Node_CallFunction, FLGNodeHandler_CallFunction);
