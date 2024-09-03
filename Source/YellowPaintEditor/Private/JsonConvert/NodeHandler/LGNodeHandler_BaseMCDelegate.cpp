// #include "JsonConvert/NodeHandler/LGNodeHandler_BaseMCDelegate.h"

// #include "JsonConvert/BpJsonConvert.h"
// #include "LogicGraphActionMenuUtils.h"
// #include "JsonConvert/BpJsonConvert.h" 
// #include "BlueprintNodeSpawner.h" 
// #include "JsonObjectConverterExt.h" 

// TSharedPtr<FJsonObject> FLGNodeHandler_BaseMCDelegate::ToJson(const TObjectPtr<UEdGraphNode> _node)
// {
// 	auto jNode = FLGNodeHandler::ToJson(_node);
// 	FStructProperty *DelegateReferenceProp = FindFProperty<FStructProperty>(UK2Node_BaseMCDelegate::StaticClass(), TEXT("DelegateReference"));
// 	const void *DelegateReferencePtr = (FMemberReference*)DelegateReferenceProp->ContainerPtrToValuePtr<uint8>(_node);
// 	TSharedRef<FJsonObject> jDelegateReference = MakeShared<FJsonObject>();
// 	FJsonObjectConverterExt::UStructToJsonObject(DelegateReferenceProp->Struct, DelegateReferencePtr, jDelegateReference, 0, 0);
// 	jNode->SetObjectField("DelegateReference", jDelegateReference);
// 	return jNode;
// }

// bool FLGNodeHandler_BaseMCDelegate::ShouldExposeAction(FBlueprintActionFilter const& Filter, FBlueprintActionInfo& BlueprintAction)
// {
// 	UEdGraphNode* TemplateNode = BlueprintAction.NodeSpawner->GetTemplateNode();
// 	if (TemplateNode)
// 	{
// 		UK2Node_BaseMCDelegate* BaseMCDelegateNode = Cast<UK2Node_BaseMCDelegate>(TemplateNode);
// 		auto Property = BaseMCDelegateNode->GetProperty();
// 		if (Property)
// 		{
// 			auto OwnerClass = Property->GetOwnerClass();
// 			return !FLogicGraphActionMenuUtils::IsNonLGExportNativeClass(OwnerClass) && FLogicGraphActionMenuUtils::IsActionContextChildOf(Filter, BlueprintAction, OwnerClass);;
// 		}
// 	}
// 	return false;
// }