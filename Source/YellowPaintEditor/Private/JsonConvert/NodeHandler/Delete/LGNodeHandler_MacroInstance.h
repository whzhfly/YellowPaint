// // Fill out your copyright notice in the Description page of Project Settings.

// #pragma once

// #include <initializer_list>

// #include "BlueprintActionFilter.h"
// #include "CoreMinimal.h"
// #include "EdGraph/EdGraphSchema.h"
// #include "K2Node_MacroInstance.h"
// #include "JsonConvert/NodeHandler/LGNodeHandler.h"
// #include "UObject/NoExportTypes.h"

// #define REGISTER_LG_ALLOWED_MACRO(...) \
// 	static auto LG_CONCAT(_reg_lg_allowed_macro_, __COUNTER__) = FLGNodeHandler_MacroInstance::RegisterAllowedMacro({__VA_ARGS__})

// class YELLOWPAINTEDITOR_API FLGNodeHandler_MacroInstance : public FLGNodeHandler
// {
// public:
// 	static TSet<FString> AllowedMacros;
// 	static bool RegisterAllowedMacro(std::initializer_list<FString>);

// public:
// 	virtual TSharedPtr<FJsonObject> ToJson(const TObjectPtr<UEdGraphNode>) override;
// 	virtual bool ShouldExposeAction(FBlueprintActionFilter const& Filter, FBlueprintActionInfo& BlueprintAction) override;
// };

// REGISTER_LG_NODE_HANDLER(UK2Node_MacroInstance, FLGNodeHandler_MacroInstance);
