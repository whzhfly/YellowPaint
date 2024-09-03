// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BlueprintActionFilter.h"
#include "CoreMinimal.h"
#include "EdGraph/EdGraphSchema.h"
#include "K2Node.h"
#include "UObject/NoExportTypes.h"

class YELLOWPAINTEDITOR_API FLGNodeHandler
{
public:
	virtual TSharedPtr<FJsonObject> ToJson(const TObjectPtr<UEdGraphNode>);
	virtual bool ShouldExposeAction(FBlueprintActionFilter const &Filter, FBlueprintActionInfo &BlueprintAction);
};

template <typename KT, typename VT, typename SELF_T>
class SingletonRegistry
{
public:
	struct Register
	{
		Register(const KT key, const VT val)
		{
			SELF_T::Get()->items_.Add(key, val);
		}
	};

public:
	TMap<KT, VT> items_;
	static SELF_T *Get()
	{
		static SELF_T inst;
		return &inst;
	}

protected:
	SingletonRegistry()
	{
	}
	SingletonRegistry(const SELF_T &) = delete;
	SingletonRegistry(const SELF_T &&) = delete;
	SingletonRegistry &operator=(const SELF_T &) = delete;
};

/*
 * SingletonRegistry of LGNodeHandlers.
 */
class YELLOWPAINTEDITOR_API FLGNodeHandlerRegistry : public SingletonRegistry<UClass *, TSharedPtr<FLGNodeHandler>, FLGNodeHandlerRegistry>
{
public:
	TSharedPtr<FLGNodeHandler> GetHandler(const UClass *NodeClass, bool SearchParent = true);
};

#define LG_CONCAT_INNER(a, b) a##b
#define LG_CONCAT(a, b) LG_CONCAT_INNER(a, b)
#define REGISTER_LG_NODE_HANDLER(NodeClassName, HandlerClassName)                         \
	static FLGNodeHandlerRegistry::Register LG_CONCAT(_reg_lg_node_handler_, __COUNTER__)( \
		NodeClassName::StaticClass(), MakeShared<HandlerClassName>()                      \
	);
