#include "JsonConvert/NodeHandler/LGNodeHandler_PromotableOperator.h"

#include "JsonConvert/BpJsonConvert.h"

TSharedPtr<FJsonObject> FLGNodeHandler_PromotableOperator::ToJson(const TObjectPtr<UEdGraphNode> _node)
{
	auto jNode = FLGNodeHandler_CallFunction::ToJson(_node);
	auto node = Cast<UK2Node_PromotableOperator>(_node);
	jNode->SetStringField("operation_name", node->GetOperationName().ToString());
	return jNode;
}

bool FLGNodeHandler_PromotableOperator::ShouldExposeAction(FBlueprintActionFilter const& Filter, FBlueprintActionInfo& BlueprintAction)
{
	return true;
}