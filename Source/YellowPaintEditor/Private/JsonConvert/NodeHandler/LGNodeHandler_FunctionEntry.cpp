#include "JsonConvert/NodeHandler/LGNodeHandler_FunctionEntry.h"

#include "JsonConvert/BpJsonConvert.h"
#include "JsonConvert/JsonObjectConverterExt.h"

TSharedPtr<FJsonObject> FLGNodeHandler_FunctionEntry::ToJson(const TObjectPtr<UEdGraphNode> _node)
{
	auto jNode = FLGNodeHandler::ToJson(_node);
	auto node = Cast<UK2Node_FunctionEntry>(_node);

	UFunction *signatureFunction = node->FindSignatureFunction();
	jNode->SetStringField("func_name", signatureFunction->GetName());

	// 导出本地变量默认值
	// 参考bool UK2Node_FunctionEntry::UpdateDefaultsFromVariableStruct(const UStruct* VariableStruct, uint8* VariableStructData)的调用
	// TODO GetFunctionVariableCache参数要不要true
	auto jLocalVariableCache = MakeShared<FJsonObject>();
	if (!node->LocalVariables.IsEmpty())
	{
		auto functionVariableCache = node->GetFunctionVariableCache();
		auto exportCB = FJsonObjectConverter::CustomExportCallback::CreateStatic(BpJsonConvert::FJsonObjectConverterExportCB);
		FJsonObjectConverterExt::UStructToJsonObject(
			functionVariableCache->GetStruct(), functionVariableCache->GetStructMemory(), jLocalVariableCache, 0, 0, &exportCB
		);
	}

	// 要导出类型和序列化后的默认值
	auto jLocalVariables = MakeShared<FJsonObject>();
	for (auto &varDesc : node->LocalVariables)
	{
		auto varName = varDesc.VarName.ToString();
		auto jVal = jLocalVariableCache->TryGetField(varName);
		auto jObj = MakeShared<FJsonObject>();
		jObj->SetStringField("name", varName);
		jObj->SetObjectField("type", BpJsonConvert::PinTypeToJson(varDesc.VarType));
		jObj->SetField("default", jVal);
		jLocalVariables->SetObjectField(varName, jObj);
	}
	jNode->SetObjectField("local_variables", jLocalVariables);
	return jNode;
}

bool FLGNodeHandler_FunctionEntry::ShouldExposeAction(FBlueprintActionFilter const& Filter, FBlueprintActionInfo& BlueprintAction)
{
	// don't reject anything for default.
	return true;
}