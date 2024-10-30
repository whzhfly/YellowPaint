#include "LogicFlowDriverInstance.h"
#include "Kismet2/CompilerResultsLog.h"
/*#include "Kismet2/KismetEditorUtilities.h"
#include*/



ULogicFlowDriverInstance::ULogicFlowDriverInstance(const FObjectInitializer& ObjectInitializer):
	Super(ObjectInitializer)
{
}

void ULogicFlowDriverInstance::InitAsset()
{
	
}


void ULogicFlowDriverInstance::DeepCopyFormAnother(ULogicFlowDriverInstance* Instance)
{
	if (Instance != nullptr)
	{
		/*HasBeenChecked = Instance->HasBeenChecked;
		ExeCount = Instance->ExeCount;*/
		/*UEditorEngine::FCopyPropertiesForUnrelatedObjectsParams CopyDetails;
		CopyDetails.bCopyDeprecatedProperties = Blueprint->bIsRegeneratingOnLoad;
		CopyDetails.bNotifyObjectReplacement = true; 
		UEditorEngine::CopyPropertiesForUnrelatedObjects(OldCDO, NewCDO, CopyDetails);*/
	}
}

void ULogicFlowDriverInstance::StartFlow()
{
	
}

ULogicSkillFlowDriver::ULogicSkillFlowDriver(const FObjectInitializer& ObjectInitializer):
	Super(ObjectInitializer)
{
	DriverType = EFlowDriverType::Skill;
}

ULogicBuffFlowDriver::ULogicBuffFlowDriver(const FObjectInitializer& ObjectInitializer):
	Super(ObjectInitializer)
{
	DriverType = EFlowDriverType::Buff;
}

ULogicTriggerFlowDriver::ULogicTriggerFlowDriver(const FObjectInitializer& ObjectInitializer):
	Super(ObjectInitializer)
{
	DriverType = EFlowDriverType::Trigger;
}

