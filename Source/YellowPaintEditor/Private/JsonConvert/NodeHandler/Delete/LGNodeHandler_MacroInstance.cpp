/*
#include "JsonConvert/NodeHandler/LGNodeHandler_MacroInstance.h"

#include "JsonConvert/BpJsonConvert.h"
#include "BlueprintNodeSpawner.h" 

TSet<FString> FLGNodeHandler_MacroInstance::AllowedMacros;

bool FLGNodeHandler_MacroInstance::RegisterAllowedMacro(std::initializer_list<FString> macros)
{
	for (auto name : macros)
	{
		FLGNodeHandler_MacroInstance::AllowedMacros.Add(name);
	}
	return true;
}

TSharedPtr<FJsonObject> FLGNodeHandler_MacroInstance::ToJson(const TObjectPtr<UEdGraphNode> _node)
{
	auto jNode = FLGNodeHandler::ToJson(_node);
	auto node = Cast<UK2Node_MacroInstance>(_node);
	auto jMacroRef = MakeShared<FJsonObject>();
	jMacroRef->SetStringField("graph", node->GetMacroGraph()->GetName());
	jMacroRef->SetStringField("blueprint", node->GetSourceBlueprint()->GetName());
	jNode->SetObjectField("macro_ref", jMacroRef);
	return jNode;
}

bool FLGNodeHandler_MacroInstance::ShouldExposeAction(FBlueprintActionFilter const& Filter, FBlueprintActionInfo& BlueprintAction)
{
	UEdGraphNode* TemplateNode = BlueprintAction.NodeSpawner->GetTemplateNode();
	if (TemplateNode)
	{
		auto RealTemplateNode = Cast<UK2Node_MacroInstance>(TemplateNode);
		FString MacroName =
			FString::Printf(TEXT("%s.%s"), *RealTemplateNode->GetSourceBlueprint()->GetName(), *RealTemplateNode->GetMacroGraph()->GetName());
		// TODO Expose custom macros
		return FLGNodeHandler_MacroInstance::AllowedMacros.Contains(MacroName);
	}
	return false;
}
*/
