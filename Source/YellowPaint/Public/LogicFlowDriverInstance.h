#pragma once



#include "CoreMinimal.h"
#include "LogicFlowNode.h"
#include "LogicFlowDriverInstance.generated.h"


/**
 * 逻辑执行类
 * 保存所有的Node
 * 形成一个Class
 */
UCLASS(Blueprintable, BlueprintType)
class YELLOWPAINT_API ULogicFlowDriverInstance : public UObject
{
public:
	GENERATED_BODY()


	UPROPERTY(EditAnywhere, DisplayName="是否激活Flow")
	bool HasBeenChecked;

	UPROPERTY(EditAnywhere, DisplayName="Flow次数")
	int ExeCount;


	UPROPERTY()
	TMap<FString, ULogicFlowNode*> FlowNodesMap;

	UFUNCTION()
	virtual void StartFlow();
	
	virtual void DeepCopyFormAnother(ULogicFlowDriverInstance* Instance);
};
