#include "JsonConvert/NodeHandler/LGNodeHandler_Event.h"

#include "JsonConvert/BpJsonConvert.h"
/*#include "LogicGraphActionMenuUtils.h"*/

TSharedPtr<FJsonObject> FLGNodeHandler_Event::ToJson(const TObjectPtr<UEdGraphNode> _node)
{
	auto jNode = FLGNodeHandler::ToJson(_node);
	auto node = Cast<UK2Node_Event>(_node);
	UFunction *eventSignatureFunction = node->FindEventSignatureFunction();
	jNode->SetStringField("event_name", eventSignatureFunction->GetName());
	return jNode;
}

bool FLGNodeHandler_Event::ShouldExposeAction(FBlueprintActionFilter const& Filter, FBlueprintActionInfo& BlueprintAction)
{
	return true;
	/*const UFunction* Func = BlueprintAction.GetAssociatedFunction();
	if (!Func) return false;
	const UClass* OwnerClass = Func->GetOwnerClass();
	return !FLogicGraphActionMenuUtils::IsNonLGExportNativeClass(OwnerClass) && FLogicGraphActionMenuUtils::IsActionContextChildOf(Filter, BlueprintAction, OwnerClass);*/
}
