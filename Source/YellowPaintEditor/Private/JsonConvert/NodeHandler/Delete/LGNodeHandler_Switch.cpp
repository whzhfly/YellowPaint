// #include "JsonConvert/NodeHandler/LGNodeHandler_Switch.h"

// #include "JsonConvert/BpJsonConvert.h"

// TSharedPtr<FJsonObject> FLGNodeHandler_Switch::ToJson(const TObjectPtr<UEdGraphNode> _node)
// {
// 	auto jNode = FLGNodeHandler::ToJson(_node);
// 	auto node = Cast<UK2Node_Switch>(_node);
// 	jNode->SetBoolField("has_default_pin", node->bHasDefaultPin);
// 	return jNode;
// }

// bool FLGNodeHandler_Switch::ShouldExposeAction(FBlueprintActionFilter const& Filter, FBlueprintActionInfo& BlueprintAction)
// {
// 	// don't reject anything for default.
// 	return false;
// }