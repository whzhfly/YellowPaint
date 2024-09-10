#include "Graph/Widgets//YellowPaintGraphNodeFactory.h"
#include "KismetPins/SGraphPinExec.h"
#include "KismetPins/SGraphPinBool.h"
#include "KismetPins/SGraphPinInteger.h"
#include "KismetPins/SGraphPinColor.h"
#include "KismetPins/SGraphPinEnum.h"
#include "KismetPins/SGraphPinExec.h"
#include "KismetPins/SGraphPinInteger.h"
#include "KismetPins/SGraphPinNum.h"
#include "KismetPins/SGraphPinObject.h"
#include "KismetPins/SGraphPinString.h"
#include "Graph//YelloPaintSchema.h"


TSharedPtr<class SGraphNode> FYellowPaintGraphNodeFactory::CreateNode(UEdGraphNode* Node) const
{
	if(UEdYellowPaintNode* GraphNode = Cast<UEdYellowPaintNode>(Node))
	{
		return SNew(SYellowPaintGraphNode, GraphNode);
	}
	return nullptr;
}


TSharedPtr<class SGraphPin> FYellowPaintGraphPinFactory::CreatePin(class UEdGraphPin* InPin) const
{
	/*if (InPin->GetSchema()->IsA<UYelloPaintSchema>() && InPin->PinType.PinCategory == UEdGraphSchema_K2::PC_Struct)
	{
		if ((InPin->PinType.PinSubCategoryObject == FPoseLink::StaticStruct()) || (InPin->PinType.PinSubCategoryObject == FComponentSpacePoseLink::StaticStruct()))
		{
			return SNew(SGraphPinPose, InPin);
		}
	}*/

	if (InPin->GetSchema()->IsA<UYelloPaintSchema>())
	{

		if (InPin->PinType.PinCategory == UEdGraphSchema_K2::PC_Boolean)
		{
			return SNew(SGraphPinBool, InPin);
		}
		else if(InPin->PinType.PinCategory == UEdGraphSchema_K2::PC_Exec)
		{
			return SNew(SGraphPinExec, InPin);
		}
		else if (InPin->PinType.PinCategory == UEdGraphSchema_K2::PC_SoftObject)
		{
			return SNew(SGraphPinObject, InPin);
		}
		/*else if (InPin->PinType.PinCategory == UEdGraphSchema_K2::PC_Class)
		{
			return SNew(SGraphPinClass, InPin);
		}
		else if (InPin->PinType.PinCategory == UEdGraphSchema_K2::PC_SoftClass)
		{
			return SNew(SGraphPinClass, InPin);
		}*/
		else if (InPin->PinType.PinCategory == UEdGraphSchema_K2::PC_Int)
		{
			return SNew(SGraphPinInteger, InPin);
		}
		else if (InPin->PinType.PinCategory == UEdGraphSchema_K2::PC_Int64)
		{
			return SNew(SGraphPinNum<int64>, InPin);
		}
		else if (InPin->PinType.PinCategory == UEdGraphSchema_K2::PC_Real)
		{
			return SNew(SGraphPinNum<double>, InPin);
		}
		return SNew(SGraphPin, InPin);
	}
	return nullptr;
}