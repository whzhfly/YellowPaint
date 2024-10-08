#include "JsonConvert/NodeHandler/LGNodeHandler.h"
#include "JsonConvert/BpJsonConvert.h"
#include "UObject/NoExportTypes.h"
#include "Graph/Nodes/EdYellowPaintNode.h"
#include  "JsonConvert/JsonObjectConverterExt.h"
#include  "JsonConvert/NodeHandler/LGNodeHandler_Default.h"

TSharedPtr<FJsonObject> FLGNodeHandler::ToJson(const TObjectPtr<UEdGraphNode> node)
{
	auto jNode = MakeShared<FJsonObject>();
	auto nodeClass = node->GetClass();

	jNode->SetStringField("guid", node->NodeGuid.ToString());
	/*jNode->SetStringField("type", nodeClass->GetName());*/


	if (Cast<UEdYellowPaintNode>(node))
	{
		UEdYellowPaintNode* PaintNode = Cast<UEdYellowPaintNode>(node);
		auto FlowProperties = MakeShared<FJsonObject>();
		ULogicFlowNode* FlowNode = PaintNode->FlowNode;
		FJsonObjectConverterExt::UStructToJsonObject(FlowNode->GetClass(), FlowNode, FlowProperties, 0, 0);
		jNode->SetStringField("type", PaintNode->FlowNode.GetClass()->GetName());
		jNode->SetObjectField("Property", FlowProperties);
	}
	
	// pins
	TArray<TSharedPtr<FJsonValue>> jPins;
	for (auto pin : node->Pins)
	{
		if (pin->bHidden && pin->SubPins.IsEmpty()) continue;
		auto jPin = BpJsonConvert::PinToJson(pin);
		jPins.Add(MakeShared<FJsonValueObject>(jPin));
	}
	jNode->SetArrayField("pins", jPins);
	return jNode;
}

bool FLGNodeHandler::ShouldExposeAction(FBlueprintActionFilter const& Filter, FBlueprintActionInfo& BlueprintAction)
{
	return true;
}

TSharedPtr<FLGNodeHandler> FLGNodeHandlerRegistry::GetHandler(const UClass* NodeClass, bool SearchParent)
{
	auto Handler = items_.Find(NodeClass);
	if (Handler != nullptr) return *Handler;

	if (SearchParent)
	{
		auto SuperClass = NodeClass->GetSuperClass();
		while (SuperClass != nullptr)
		{
			Handler = items_.Find(SuperClass);
			if (Handler != nullptr) return *Handler;
			SuperClass = SuperClass->GetSuperClass();
		}
	}
	// default
	const UClass* key = UK2Node::StaticClass();
	return *items_.Find(key);
}