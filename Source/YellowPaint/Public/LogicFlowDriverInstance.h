#pragma once



#include "CoreMinimal.h"
#include "LogicFlowNode.h"
#include "LogicFlowDriverInstance.generated.h"



/** Enum used to define which way data flows into or out of this pin. */
UENUM(BlueprintType)
enum class EFlowDriverType
{
	Default = 0 UMETA(DisplayName = "默认"),
	Skill = 1 UMETA(DisplayName = "技能"),
	Buff = 2 UMETA(DisplayName = "Buff"),
	Trigger = 3 UMETA(DisplayName = "触发器"),
	Other = 9 UMETA(DisplayName = "其他"),
};


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

	UPROPERTY(VisibleAnywhere, DisplayName="资源类型", Category="LogicFlow")
	EFlowDriverType DriverType = EFlowDriverType::Default;


	UPROPERTY(EditAnywhere, DisplayName="模板可被继承", AdvancedDisplay, meta=(ShowOnlyInnerProperties))
	bool TemplateFlag = true;

	UFUNCTION()
	static bool IsTemplate(){ return false;};

	UPROPERTY()
	TMap<FString, ULogicFlowNode*> FlowNodesMap;

	UFUNCTION()
	virtual void StartFlow();
	
	virtual void DeepCopyFormAnother(ULogicFlowDriverInstance* Instance);
};
