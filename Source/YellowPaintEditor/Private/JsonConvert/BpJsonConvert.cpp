#include "BpJsonConvert.h"


// Fill out your copyright notice in the Description page of Project Settings.

#include "BpJsonConvert.h"

#include "EdGraphNode_Comment.h"
#include "K2Node_AddDelegate.h"
#include "K2Node_AssignDelegate.h"
#include "K2Node_AssignmentStatement.h"
#include "K2Node_AsyncAction.h"
#include "K2Node_CallArrayFunction.h"
#include "K2Node_CallDelegate.h"
#include "K2Node_CallFunction.h"
#include "K2Node_CommutativeAssociativeBinaryOperator.h"
#include "K2Node_ComponentBoundEvent.h"
#include "K2Node_CustomEvent.h"
#include "K2Node_DynamicCast.h"
#include "K2Node_Event.h"
#include "K2Node_ExecutionSequence.h"
#include "K2Node_FunctionEntry.h"
#include "K2Node_FunctionResult.h"
#include "K2Node_GetArrayItem.h"
#include "K2Node_IfThenElse.h"
#include "K2Node_Knot.h"
#include "K2Node_MacroInstance.h"
#include "K2Node_PromotableOperator.h"
#include "K2Node_SwitchEnum.h"
#include "K2Node_SwitchInteger.h"
#include "K2Node_SwitchName.h"
#include "K2Node_SwitchString.h"
#include "K2Node_TemporaryVariable.h"
#include "K2Node_VariableGet.h"
#include "K2Node_VariableSet.h"
// Kismet
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetTextLibrary.h"
// custom
#include <functional>

#include "JsonObjectConverterExt.h"
#include "JsonUtilities.h"
/*#include "K2Node_Normal.h"*/
#include "Misc/DefaultValueHelper.h"
#include "NodeHandler/LGNodeHandler.h"

BpJsonConvert::BpJsonConvert(UBlueprint *bp) : bp_(bp)
{
}

BpJsonConvert::~BpJsonConvert()
{
}

TSharedPtr<FJsonValue> BpJsonConvert::FJsonObjectConverterExportCB(FProperty *property, const void *value)
{
	/**
	 * Optional callback that will be run when exporting a single property to Json.
	 * If this returns a valid value it will be inserted into the export chain.
	 * If this returns nullptr or is not bound, it will try generic type-specific export behavior before falling back to outputting ExportText as a
	 * string.
	 */
	// ue导出enum时，存的是字符串名称（其实这是合理的），而我们存的是value，后面再看要不要改
	// 注意：enum可能是FEnumProperty或FByteProperty，FByteProperty估计是老的写法
	if (property->IsA<FEnumProperty>())
	{
		FEnumProperty *enumProperty = CastField<FEnumProperty>(property);
		auto realVal = enumProperty->GetUnderlyingProperty()->GetSignedIntPropertyValue(value);
		return MakeShared<FJsonValueNumber>(realVal);
	}
	else if (auto byteProperty = CastField<const FByteProperty>(property))
	{
		if (byteProperty->Enum != nullptr)
		{
			return MakeShared<FJsonValueNumber>(*(int *)value);
		}
	}
	return nullptr;	 // 其他情况返回nullptr走默认导出逻辑
}

TSharedPtr<FJsonObject> BpJsonConvert::Convert()
{
	auto jbp = MakeShared<FJsonObject>();
	UClass *bpClass = bp_->GeneratedClass;
	UObject *bpCDO = bpClass->GetDefaultObject();
	jbp->SetStringField("parent_class", bp_->ParentClass->GetPathName());
	jbp->SetStringField("class_name", bp_->GetName());
	jbp->SetStringField("class_path", bpClass->GetPathName());

	// properties
	auto jProperties = MakeShared<FJsonObject>();
	// 使用ue自带的json导出方法
	auto exportCB = FJsonObjectConverter::CustomExportCallback::CreateStatic(BpJsonConvert::FJsonObjectConverterExportCB);
	FJsonObjectConverterExt::UStructToJsonObject(bpClass, bpCDO, jProperties, 0, 0, &exportCB);
	jbp->SetObjectField("properties", jProperties);
	// 默认去掉非MOS_EXPORT的属性
	TArray<FString> keyToRemove;
	for (auto &elem : jProperties->Values)
	{
		auto p = bpClass->FindPropertyByName(FName(elem.Key));
		if (!p) continue;
		if (p->GetOwnerClass()->HasAnyClassFlags(CLASS_Native) && !p->HasMetaData("MOS_EXPORT"))
		{
			keyToRemove.Add(elem.Key);
			continue;
		}
		auto pObject = CastField<const FObjectPropertyBase>(p);
		if (pObject && pObject->PropertyClass->IsChildOf(UActorComponent::StaticClass()))
		{
			keyToRemove.Add(elem.Key);
			continue;
		}
	}
	for (auto &key : keyToRemove)
	{
		jProperties->RemoveField(key);
	}

	// NewVariables
	auto jNewVariables = MakeShared<FJsonObject>();
	for (auto &varDesc : bp_->NewVariables)
	{
		// name
		auto varName = varDesc.VarName.ToString();
		auto jval = jProperties->TryGetField(varName);
		if (!jval) continue;

		auto jObj = MakeShared<FJsonObject>();
		jObj->SetStringField("name", varName);
		jObj->SetObjectField("type", PinTypeToJson(varDesc.VarType));
		jObj->SetField("default", jval);

		jProperties->RemoveField(varName);
		jNewVariables->SetObjectField(varName, jObj);
	}
	jbp->SetObjectField("new_variables", jNewVariables);

	// Eveng Graphs
	TArray<UEdGraph *> allEventGraphs;
	for (UEdGraph *UberGraph : bp_->UbergraphPages)
	{
		allEventGraphs.Add(UberGraph);
		UberGraph->GetAllChildrenGraphs(allEventGraphs);
	}
	TArray<TSharedPtr<FJsonValue>> jEventGraphs;
	for (const UEdGraph *graph : allEventGraphs)
	{
		if (graph->Nodes.IsEmpty()) continue;
		auto jGraph = GraphToJson(graph);
		jEventGraphs.Add(MakeShared<FJsonValueObject>(jGraph));
	}
	jbp->SetArrayField("event_graphs", jEventGraphs);

	// Function Graphs
	TArray<TSharedPtr<FJsonValue>> jFuncGraphs;
	for (auto graph : bp_->FunctionGraphs)
	{
		auto jGraph = GraphToJson(graph);
		jFuncGraphs.Add(MakeShared<FJsonValueObject>(jGraph));
	}
	jbp->SetArrayField("func_graphs", jFuncGraphs);

	return jbp;
}

FString BpJsonConvert::ConvertToStr()
{
	FString jsonStr;
	auto json = Convert();
	auto Writer = TJsonWriterFactory<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>::Create(&jsonStr);
	FJsonSerializer::Serialize(json.ToSharedRef(), Writer);
	return jsonStr;
}

TSharedPtr<FJsonObject> BpJsonConvert::NodeToJson(const TObjectPtr<UEdGraphNode> node)
{
	auto nodeClass = node->GetClass();
	auto handler = FLGNodeHandlerRegistry::Get()->GetHandler(nodeClass);
	/*if (!handler) return nullptr;*/
	TSharedPtr<FJsonObject> jNode = handler->ToJson(node);
	return jNode;
}

TSharedPtr<FJsonObject> BpJsonConvert::FEdGraphTerminalTypeToJson(const FEdGraphTerminalType &tt)
{
	auto jtt = MakeShared<FJsonObject>();
	jtt->SetStringField("type", tt.TerminalCategory.ToString());

	if (tt.TerminalCategory == UEdGraphSchema_K2::PC_Object || tt.TerminalCategory == UEdGraphSchema_K2::PC_SoftObject ||
		tt.TerminalCategory == UEdGraphSchema_K2::PC_Class || tt.TerminalCategory == UEdGraphSchema_K2::PC_SoftClass ||
		tt.TerminalCategory == UEdGraphSchema_K2::PC_Struct)
	{
		if (tt.TerminalSubCategory == UEdGraphSchema_K2::PN_Self)
		{
			jtt->SetStringField("class_name", "");
		}
		else
		{
			UStruct *cls = Cast<UStruct>(tt.TerminalSubCategoryObject);
			jtt->SetStringField("class_name", cls->GetName());
		}
	}
	else if (tt.TerminalCategory == UEdGraphSchema_K2::PC_Enum)
	{
		const UEnum *enumType = Cast<const UEnum>(tt.TerminalSubCategoryObject);
		jtt->SetStringField("class_name", enumType->GetName());
	}
	else if (tt.TerminalCategory == UEdGraphSchema_K2::PC_Byte)
	{
		const UEnum *enumType = Cast<const UEnum>(tt.TerminalSubCategoryObject);
		if (enumType != nullptr)
		{
			jtt->SetStringField("type", "enum");
			jtt->SetStringField("class_name", enumType->GetName());
		}
	}
	return jtt;
}

TSharedPtr<FJsonObject> BpJsonConvert::PinTypeToJson(const FEdGraphPinType &pt, bool no_container)
{
	auto jpt = MakeShared<FJsonObject>();
	// auto exportCB = FJsonObjectConverter::CustomExportCallback::CreateStatic(BpJsonConvert::FJsonObjectConverterExportCB);
	// FJsonObjectConverter::UStructToJsonObject(pinType.StaticStruct(), &pinType, jPinType, 0, 0, &exportCB);
	// return jPinType;

	if (!no_container && pt.IsContainer())
	{
		if (pt.IsArray())
		{
			jpt->SetStringField("type", "array");
			jpt->SetObjectField("value_type", BpJsonConvert::PinTypeToJson(pt, true));
		}
		else if (pt.IsSet())
		{
			jpt->SetStringField("type", "set");
			jpt->SetObjectField("value_type", BpJsonConvert::PinTypeToJson(pt, true));
		}
		else if (pt.IsMap())
		{
			jpt->SetStringField("type", "map");
			jpt->SetObjectField("key_type", BpJsonConvert::PinTypeToJson(pt, true));
			jpt->SetObjectField("value_type", BpJsonConvert::FEdGraphTerminalTypeToJson(pt.PinValueType));
		}
		return jpt;
	}
	jpt->SetStringField("type", pt.PinCategory.ToString());
	if (pt.PinCategory == UEdGraphSchema_K2::PC_Object || pt.PinCategory == UEdGraphSchema_K2::PC_SoftObject ||
		pt.PinCategory == UEdGraphSchema_K2::PC_Class || pt.PinCategory == UEdGraphSchema_K2::PC_SoftClass ||
		pt.PinCategory == UEdGraphSchema_K2::PC_Struct)
	{
		if (pt.PinSubCategory == UEdGraphSchema_K2::PN_Self)
		{
			jpt->SetStringField("class_name", "");
		}
		else
		{
			UStruct *cls = Cast<UStruct>(pt.PinSubCategoryObject);
			if (cls == UObject::StaticClass())
			{
				jpt->SetStringField("class_name", "object");  // 对应到python的object上
			}
			else
			{
				jpt->SetStringField("class_name", cls->GetName());
			}
		}
	}
	else if (pt.PinCategory == UEdGraphSchema_K2::PC_Delegate)
	{
		const FSimpleMemberReference &mref = pt.PinSubCategoryMemberReference;
		FString member_name = mref.MemberName.ToString();
		member_name.RemoveFromStart("Delegate_");
		member_name.RemoveFromEnd("__DelegateSignature");
		jpt->SetStringField("member_name", member_name);
		jpt->SetStringField("member_parent", mref.MemberParent.GetName());
	}
	else if (pt.PinCategory == UEdGraphSchema_K2::PC_Enum)
	{
		const UEnum *enumType = Cast<const UEnum>(pt.PinSubCategoryObject);
		jpt->SetStringField("class_name", enumType->GetName());
	}
	else if (pt.PinCategory == UEdGraphSchema_K2::PC_Byte)
	{
		const UEnum *enumType = Cast<const UEnum>(pt.PinSubCategoryObject);
		if (enumType != nullptr)
		{
			jpt->SetStringField("type", "enum");
			jpt->SetStringField("class_name", enumType->GetName());
		}
	}
	return jpt;
}

TSharedPtr<FJsonValue> BpJsonConvert::PinDefaultValueToJson(const UEdGraphPin *pin)
{
	auto pinCategory = pin->PinType.PinCategory;
	if (pin->PinType.IsContainer() || pin->PinType.PinCategory == UEdGraphSchema_K2::PC_Exec)
	{
		return MakeShared<FJsonValueNull>();
	}
	FString defaultStr = pin->GetDefaultAsString();
	if (pinCategory == UEdGraphSchema_K2::PC_String || pinCategory == UEdGraphSchema_K2::PC_Name || pinCategory == UEdGraphSchema_K2::PC_Text)
	{
		return MakeShared<FJsonValueString>(defaultStr);
	}
	else if (pinCategory == UEdGraphSchema_K2::PC_Boolean)
	{
		return MakeShared<FJsonValueBoolean>(defaultStr == "true");
	}
	else if (pinCategory == UEdGraphSchema_K2::PC_Float || pinCategory == UEdGraphSchema_K2::PC_Double || pinCategory == UEdGraphSchema_K2::PC_Real)
	{
		auto val = defaultStr.IsEmpty() ? 0.0 : FCString::Atof(*defaultStr);
		return MakeShared<FJsonValueNumber>(val);
	}
	else if (pinCategory == UEdGraphSchema_K2::PC_Int || pinCategory == UEdGraphSchema_K2::PC_Int64)
	{
		auto val = defaultStr.IsEmpty() ? 0.0 : FCString::Atoi64(*defaultStr);
		return MakeShared<FJsonValueNumber>(val);
	}
	else if (pinCategory == UEdGraphSchema_K2::PC_Enum)
	{
		auto val = 0;
		if (!defaultStr.IsEmpty())
		{
			const UEnum *enumType = Cast<const UEnum>(pin->PinType.PinSubCategoryObject);
			val = enumType->GetValueByNameString(defaultStr);
		}
		return MakeShared<FJsonValueNumber>(val);
	}
	else if (pinCategory == UEdGraphSchema_K2::PC_Byte)
	{
		auto val = 0;
		if (!defaultStr.IsEmpty())
		{
			const UEnum *enumType = Cast<const UEnum>(pin->PinType.PinSubCategoryObject);
			if (enumType == nullptr)
			{
				val = FCString::Atoi(*defaultStr);
			}
			else
			{
				val = enumType->GetValueByNameString(defaultStr);
			}
		}
		return MakeShared<FJsonValueNumber>(val);
	}
	else if (pinCategory == UEdGraphSchema_K2::PC_Struct)
	{
		const UScriptStruct *structType = Cast<const UScriptStruct>(pin->PinType.PinSubCategoryObject);
		auto VectorStruct = TBaseStructure<FVector>::Get();
		auto Vector3fStruct = TVariantStructure<FVector3f>::Get();
		auto RotatorStruct = TBaseStructure<FRotator>::Get();
		auto TransformStruct = TBaseStructure<FTransform>::Get();
		auto LinearColorStruct = TBaseStructure<FLinearColor>::Get();
		auto ColorStruct = TBaseStructure<FColor>::Get();
		void *val = nullptr;
		if ((structType == VectorStruct) || (structType == Vector3fStruct))
		{
			FVector vec;
			FDefaultValueHelper::ParseVector(defaultStr, vec);
			val = &vec;
		}
		else if (structType == RotatorStruct)
		{
			FRotator rot;
			FDefaultValueHelper::ParseRotator(defaultStr, rot);
			val = &rot;
		}
		else if (structType == TransformStruct)
		{
			FTransform transform;
			transform.InitFromString(defaultStr);
			val = &transform;
		}
		else if (structType == LinearColorStruct)
		{
			FLinearColor color;
			color.InitFromString(defaultStr);
			val = &color;
		}
		else
		{
			// Structs must pass validation at this point, because we need a FStructProperty to run ImportText
			// They'll be verified in FKCHandler_CallFunction::CreateFunctionCallStatement()
			return MakeShared<FJsonValueString>(defaultStr);
		}
		auto jObj = MakeShared<FJsonObject>();
		auto exportCB = FJsonObjectConverter::CustomExportCallback::CreateStatic(BpJsonConvert::FJsonObjectConverterExportCB);
		FJsonObjectConverterExt::UStructToJsonObject(structType, val, jObj, 0, 0, &exportCB);
		return MakeShared<FJsonValueObject>(jObj);
	}
	else
	{
		// 默认导出为字符串
		return MakeShared<FJsonValueString>(defaultStr);
	}
}

TSharedPtr<FJsonObject> BpJsonConvert::PinToJson(const UEdGraphPin *pin)
{
	auto jPin = MakeShared<FJsonObject>();
	FString pinName = pin->GetName();

	// subpin的name会有parentName_前缀
	// Engine\Source\Editor\BlueprintGraph\Private\K2Node.cpp, UK2Node::GetPinMetaData
	if (pin->ParentPin && pin->ParentPin->PinType.PinCategory == UEdGraphSchema_K2::PC_Struct)
	{
		pinName = pin->GetName().RightChop(pin->ParentPin->GetName().Len() + 1);
	}
	jPin->SetStringField("guid", pin->PinId.ToString());
	jPin->SetStringField("name", pinName);
	jPin->SetObjectField("type", PinTypeToJson(pin->PinType));
	jPin->SetNumberField("direction", static_cast<uint8>(pin->Direction.GetValue()));

	// input pin可以直接填参数，需要导出默认值
	if (pin->Direction == EEdGraphPinDirection::EGPD_Input)
	{
		if (!pin->HasAnyConnections())
		{
			jPin->SetField("value", BpJsonConvert::PinDefaultValueToJson(pin));
		}
		if (pin->PinName == UEdGraphSchema_K2::PN_Self)
		{
			jPin->SetBoolField("defaultToSelf", true);
		}
	}

	// 节点连接关系导出
	// - input和output端口的连接关系是相互的，只需导出到一端，这里统一导出到output端口
	// - UK2Node_Knot会展开，导出的结构中不存在UK2Node_Knot
	std::function<void(TArray<TSharedPtr<FJsonValue>> &, const UEdGraphPin *)> linkToInputPinFunc;
	linkToInputPinFunc = [&linkToInputPinFunc](TArray<TSharedPtr<FJsonValue>> &jLinkedTo, const UEdGraphPin *toPin)
	{
		if (toPin->GetOwningNode()->GetClass() == UK2Node_Knot::StaticClass())
		{
			auto toNode = Cast<UK2Node_Knot>(toPin->GetOwningNode());
			for (auto pin : toNode->GetOutputPin()->LinkedTo)
			{
				linkToInputPinFunc(jLinkedTo, pin);
			}
		}
		else
		{
			auto jLinkedToPin = MakeShared<FJsonObject>();
			jLinkedToPin->SetStringField("node", toPin->GetOwningNode()->NodeGuid.ToString());
			jLinkedToPin->SetStringField("pin", toPin->PinId.ToString());
			jLinkedTo.Add(MakeShared<FJsonValueObject>(jLinkedToPin));
		}
	};
	TArray<TSharedPtr<FJsonValue>> jLinkedTo;
	if (pin->Direction == EEdGraphPinDirection::EGPD_Output)
	{
		for (auto toPin : pin->LinkedTo)
		{
			linkToInputPinFunc(jLinkedTo, toPin);
		}
	}
	jPin->SetArrayField("linked_to", jLinkedTo);

	// 导出pin的父子关系（主要用在结构体字段的展开和折叠）
	TArray<TSharedPtr<FJsonValue>> jSubPins;
	for (auto subPin : pin->SubPins)
	{
		jSubPins.Add(MakeShared<FJsonValueString>(subPin->PinId.ToString()));
	}
	jPin->SetArrayField("sub_pins", jSubPins);
	return jPin;
}

TSharedPtr<FJsonObject> BpJsonConvert::GraphToJson(const UEdGraph *graph)
{
	auto jGraph = MakeShared<FJsonObject>();
	jGraph->SetStringField("guid", graph->GraphGuid.ToString());
	TArray<TSharedPtr<FJsonValue>> jNodes;
	TArray<TSharedPtr<FJsonValue>> jConnections;
	for (const auto &node : graph->Nodes)
	{
		if (node.GetClass() == UEdGraphNode_Comment::StaticClass()) continue;  // 忽略注释节点
		if (node.GetClass() == UK2Node_Knot::StaticClass()) continue;  // 忽略UK2Node_Knot，pin连接关系导出的时候的时候会自动展开
		auto jNode = NodeToJson(node);
		jNodes.Add(MakeShared<FJsonValueObject>(jNode));
	}
	jGraph->SetArrayField("nodes", jNodes);
	jGraph->SetArrayField("connections", jConnections);
	return jGraph;
}
