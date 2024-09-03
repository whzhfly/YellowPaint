#include "JsonConvert/NodeHandler/LGNodeHandler_StructOperation.h"
#include "BlueprintNodeSpawner.h"
#include "JsonConvert/BpJsonConvert.h"
/*#include "LogicGraphActionMenuUtils.h"*/

TSet<FString> FLGNodeHandler_StructOperation::AllowedStruct;

bool FLGNodeHandler_StructOperation::RegisterAllowedStruct(std::initializer_list<FString> StructNames)
{
	for (auto Name : StructNames)
	{
		FLGNodeHandler_StructOperation::AllowedStruct.Add(Name);
	}
	return true;
}

TSharedPtr<FJsonObject> FLGNodeHandler_StructOperation::ToJson(const TObjectPtr<UEdGraphNode> _node)
{
	auto jNode = FLGNodeHandler::ToJson(_node);
	return jNode;
}

bool FLGNodeHandler_StructOperation::ShouldExposeAction(FBlueprintActionFilter const& Filter, FBlueprintActionInfo& BlueprintAction)
{
	return true;
	/*UEdGraphNode* TemplateNode = BlueprintAction.NodeSpawner->GetTemplateNode();
	if (TemplateNode)
	{
		UK2Node_StructOperation* StructOperation = Cast<UK2Node_StructOperation>(TemplateNode);
		TObjectPtr<UScriptStruct> ScriptStruct = StructOperation->StructType;
		if (StructOperation->StructType)
		{
			if (StructOperation->StructType->HasMetaData("MOS_EXPORT")) return true;
			if (AllowedStruct.Contains(StructOperation->StructType.GetName())) return true;
		}
	}
	return false;*/
}

REGISTER_LG_ALLOWED_STRUCT("Vector", "Rotator", "LinearColor");
