#include "JsonConvert/NodeHandler/LGNodeHandler_CallFunction.h"
#include "JsonConvert/BpJsonConvert.h"
// #include "LogicGraphActionMenuUtils.h"

TSet<FString> FLGNodeHandler_CallFunction::AllowedStaticFunctions;

bool FLGNodeHandler_CallFunction::RegisterAllowedStaticFunction(std::initializer_list<FString> functions)
{
	for (auto name : functions)
	{
		FLGNodeHandler_CallFunction::AllowedStaticFunctions.Add(name);
	}
	return true;
}

bool FLGNodeHandler_CallFunction::RegisterAllowedStaticFunctionOfClass(FString OwnerClass, std::initializer_list<FString> functions)
{
	for (auto name : functions)
	{
		FLGNodeHandler_CallFunction::AllowedStaticFunctions.Add(FString::Printf(TEXT("%s.%s"), *OwnerClass, *name));
	}
	return true;
}

TSharedPtr<FJsonObject> FLGNodeHandler_CallFunction::ToJson(const TObjectPtr<UEdGraphNode> _node)
{
	auto jNode = FLGNodeHandler::ToJson(_node);
	auto node = Cast<UK2Node_CallFunction>(_node);

	UFunction* func = node->GetTargetFunction();
	auto funcRef = node->FunctionReference;

	{
		jNode->SetBoolField("is_pure", func->HasAllFunctionFlags(FUNC_BlueprintPure));
	}
	
	// 调用函数名称，可能是self.xxx，也可能是someClass.xxx
	jNode->SetStringField("func_name", func->GetName());
	if (!funcRef.IsSelfContext())
	{
		jNode->SetStringField("func_class", funcRef.GetMemberParentClass()->GetName());
	}
	else
	{
		jNode->SetStringField("func_class", "");
	}
	jNode->SetBoolField("is_static", func->HasAnyFunctionFlags(EFunctionFlags::FUNC_Static));

	const FString& DefaultToSelfMetaValue = func->GetMetaData(FBlueprintMetadata::MD_DefaultToSelf);
	const FString& WorldContextMetaValue = func->GetMetaData(FBlueprintMetadata::MD_WorldContext);
	// 覆盖
	TArray<TSharedPtr<FJsonValue>> jPins;
	for (auto pin : node->Pins)
	{
		if (pin->bHidden && pin->SubPins.IsEmpty() && pin->GetName() != WorldContextMetaValue) continue;
		auto jPin = BpJsonConvert::PinToJson(pin);
		if (pin->PinName == UEdGraphSchema_K2::PN_Self || pin->GetName() == WorldContextMetaValue || pin->GetName() == DefaultToSelfMetaValue)
		{
			jPin->SetBoolField("defaultToSelf", true);
		}
		jPins.Add(MakeShared<FJsonValueObject>(jPin));
	}
	jNode->SetArrayField("pins", jPins);
	return jNode;
}

/*bool FLGNodeHandler_CallFunction::ShouldExposeAction(FBlueprintActionFilter const& Filter, FBlueprintActionInfo& BlueprintAction)
{
	const UFunction* Func = BlueprintAction.GetAssociatedFunction();
	if (!Func) return false;

	const UClass* OwnerClass = Func->GetOwnerClass();
	if (Func->FunctionFlags & FUNC_Static)
	{
		if (!OwnerClass->HasMetaData(TEXT("MOS_EXPORT")) &&
			!AllowedStaticFunctions.Contains(FString::Printf(TEXT("%s.%s"), *OwnerClass->GetName(), *Func->GetName())))
		{
			return false;
		}
		if (Func->HasMetaData("LGRestrictedContext"))
		{
			FString const& ClassRestrictions = Func->GetMetaData("LGRestrictedContext");
			return FLogicGraphActionMenuUtils::DoesMatchClassRestriction(Filter, BlueprintAction, ClassRestrictions);
		}
		else if (OwnerClass->HasMetaData("LGRestrictedContext"))
		{
			FString const& ClassRestrictions = OwnerClass->GetMetaData("LGRestrictedContext");
			return FLogicGraphActionMenuUtils::DoesMatchClassRestriction(Filter, BlueprintAction, ClassRestrictions);
		}
		else
		{
			return true;
		}
	}
	else
	{
		return !FLogicGraphActionMenuUtils::IsNonLGExportNativeClass(OwnerClass) &&
			   FLogicGraphActionMenuUtils::IsActionContextChildOf(Filter, BlueprintAction, OwnerClass);
	}
}*/

// 注册内建支持一些宏
// REGISTER_LG_ALLOWED_STATIC_FUNCTION_OF_CLASS(
// 	"KismetSystemLibrary", "PrintString", "LogString", "MakeLiteralString", "MakeLiteralInt", "MakeLiteralDouble", "MakeLiteralBool",
// 	"FlushPersistentDebugLines"
// );

// REGISTER_LG_ALLOWED_STATIC_FUNCTION_OF_CLASS(
// 	"KismetStringLibrary", "TrimTrailing", "Trim", "ToUpper", "ToLower", "RightPad", "RightChop", "Right", "Reverse", "ParseIntoArray",
// 	"NotEqual_StrStr", "NotEqual_StriStri", "Mid", "Len", "LeftPad", "LeftChop", "Left", "JoinStringArray", "IsNumeric", "IsEmpty", "GetSubstring",
// 	"GetCharacterAsNumber", "GetCharacterArrayFromString", "FindSubstring", "EqualEqual_StrStr", "EqualEqual_StriStri", "CullArray",
// 	"Conv_StringToInt", "Conv_StringToDouble", "Conv_IntToString", "Conv_DoubleToString", "Conv_BoolToString", "Contains", "Concat_StrStr",
// 	"StartsWith", "Split", "Replace", "EndsWith"
// );
