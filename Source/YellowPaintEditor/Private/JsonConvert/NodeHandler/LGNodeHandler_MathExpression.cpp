#include "JsonConvert/NodeHandler/LGNodeHandler_MathExpression.h"

#include "JsonConvert/BpJsonConvert.h"

TSharedPtr<FJsonObject> FLGNodeHandler_MathExpression::ToJson(const TObjectPtr<UEdGraphNode> _node)
{
	auto jNode = FLGNodeHandler::ToJson(_node);
	auto node = Cast<UK2Node_MathExpression>(_node);
	jNode->SetStringField("expression", node->Expression);
	return jNode;
}

bool FLGNodeHandler_MathExpression::ShouldExposeAction(FBlueprintActionFilter const& Filter, FBlueprintActionInfo& BlueprintAction)
{
	// don't reject anything for default.
	return true;
}