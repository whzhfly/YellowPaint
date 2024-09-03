#include "JsonConvert/NodeHandler/LGNodeHandler_CustomEvent.h"

#include "JsonConvert/BpJsonConvert.h"

TSharedPtr<FJsonObject> FLGNodeHandler_CustomEvent::ToJson(const TObjectPtr<UEdGraphNode> _node)
{
	auto jNode = FLGNodeHandler::ToJson(_node);
	auto node = Cast<UK2Node_CustomEvent>(_node);
	jNode->SetStringField("event_name", node->CustomFunctionName.ToString());
	return jNode;
}

bool FLGNodeHandler_CustomEvent::ShouldExposeAction(FBlueprintActionFilter const& Filter, FBlueprintActionInfo& BlueprintAction)
{
	// don't reject anything for default.
	return true;
}