#include "LogicFlowDriverInstance.h"



void ULogicFlowDriverInstance::DeepCopyFormAnother(ULogicFlowDriverInstance* Instance)
{
	if (Instance != nullptr)
	{
		HasBeenChecked = Instance->HasBeenChecked;
		ExeCount = Instance->ExeCount;
	}
}

void ULogicFlowDriverInstance::StartFlow()
{
	
}