#include "JsonConvert/NodeHandler/LGNodeHandler_Variable.h"

#include "JsonConvert/BpJsonConvert.h"
/*#include "LogicGraphActionMenuUtils.h"*/
#include "JsonConvert/JsonObjectConverterExt.h"

TSharedPtr<FJsonObject> FLGNodeHandler_Variable::ToJson(const TObjectPtr<UEdGraphNode> _node)
{
	auto jNode = FLGNodeHandler::ToJson(_node);
	auto node = Cast<UK2Node_Variable>(_node);
	TSharedRef<FJsonObject> variableRef = MakeShared<FJsonObject>();
	FJsonObjectConverterExt::UStructToJsonObject(node->VariableReference.StaticStruct(), &node->VariableReference, variableRef, 0, 0);
	jNode->SetObjectField("variable_ref", variableRef);
	return jNode;
}

bool FLGNodeHandler_Variable::ShouldExposeAction(FBlueprintActionFilter const& Filter, FBlueprintActionInfo& BlueprintAction)
{
	return true;
	/*auto Property = BlueprintAction.GetAssociatedProperty();
	if (Property)
	{
		auto name = Property->GetName();
		const UClass* OwnerClass = Property->GetOwnerClass();
		if (!OwnerClass->HasAnyClassFlags(CLASS_Native) && name == "Target") {
			return FLogicGraphActionMenuUtils::IsActionContextChildOf(Filter, BlueprintAction, OwnerClass);
		}
		return !FLogicGraphActionMenuUtils::IsNonLGExportNativeClass(OwnerClass) &&
			   FLogicGraphActionMenuUtils::IsActionContextChildOf(Filter, BlueprintAction, OwnerClass);
	}
	return false;*/
}