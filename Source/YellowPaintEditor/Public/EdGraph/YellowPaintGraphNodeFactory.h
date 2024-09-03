#pragma once
#include "EdGraphUtilities.h"
#include "Schemal/EdYellowPaintNode.h"
#include "SYellowPaintGraphNode.h"

class FYellowPaintGraphNodeFactory : public FGraphPanelNodeFactory
{
public:
	virtual TSharedPtr<class SGraphNode> CreateNode(UEdGraphNode* Node) const override
	{
		if(UEdYellowPaintNode* GraphNode = Cast<UEdYellowPaintNode>(Node))
		{
			return SNew(SYellowPaintGraphNode, GraphNode);
		}

		return nullptr;
	}
};

class FYellowPaintGraphPinFactory : public FGraphPanelPinFactory
{
public:
	virtual TSharedPtr<class SGraphPin> CreatePin(class UEdGraphPin* InPin) const override;
};

/*class FAnimationGraphPinConnectionFactory : public FGraphPanelPinConnectionFactory
{
public:
	virtual class FConnectionDrawingPolicy* CreateConnectionPolicy(const class UEdGraphSchema* Schema, int32 InBackLayerID, int32 InFrontLayerID, float ZoomFactor, const class FSlateRect& InClippingRect, class FSlateWindowElementList& InDrawElements, class UEdGraph* InGraphObj) const override;
};*/