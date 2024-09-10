#pragma once

#include "CoreMinimal.h"
#include "Engine/BlueprintGeneratedClass.h"
#include "LogicFlowAsset.h"
#include "YellowPaintGeneratedClass.generated.h"
/**
 * 
 */

UCLASS()
class YELLOWPAINT_API UYellowPaintGeneratedClass : public UBlueprintGeneratedClass
{
	GENERATED_BODY()
public:

	/*virtual void InitializeQuest(ULogicFlowNode* Node);*/

	// UStruct interface
	virtual void Link(FArchive& Ar, bool bRelinkExistingProperties) override;
	// End of UStruct interface

	virtual void PostLoad() override;

	// UClass interface
	virtual void PurgeClass(bool bRecompilingOnLoad) override;
	// End of UClass interface

private:

	/*//The quest template to be created 
	UPROPERTY()
	UQuestLogicBase* QuestTemplate;*/
public:
	UPROPERTY()
	TObjectPtr<ULogicFlowDriverInstance> FlowDriverInstance;
};