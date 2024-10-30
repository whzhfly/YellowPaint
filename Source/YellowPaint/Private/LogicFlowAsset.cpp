// Fill out your copyright notice in the Description page of Project Settings.


#include "LogicFlowAsset.h"


ULogicFlowAsset::ULogicFlowAsset(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	/*UClass* Cls = GetBlueprintClass();
	GetParentNativeClass(Cls);*/
	UClass* DriverCls = ParentClass;
	UClass* NativeCLs = GetParentNativeClass(DriverCls);
	/*TSubclassOf<ULogicFlowDriverInstance> DriveCls = Cast<ULogicFlowDriverInstance>(Cls);*/
	/*FlowInstance = CreateDefaultSubobject<ULogicFlowDriverInstance>(TEXT("DefaultFlowInstance"));*/
	
	/*FlowInstance = static_cast<ULogicFlowDriverInstance*>(CreateDefaultSubobject(TEXT("DefaultFlowInstance"),
		Cls, Cls, true, false));*/

	if (NativeCLs)
	{
		FlowInstance = static_cast<ULogicFlowDriverInstance*>(CreateDefaultSubobject(TEXT("DefaultFlowInstance"),
	NativeCLs, NativeCLs, true, false));
	}
	else
	{
		// 通常只走这里 FlowInstance 注定无法是skii buff 等特殊类型
		FlowInstance = CreateDefaultSubobject<ULogicFlowDriverInstance>(TEXT("DefaultFlowInstance"));
	}

	
	FlowInstance->SetFlags(RF_Transactional | RF_ArchetypeObject);
}

EFlowDriverType ULogicFlowAsset::GetAssetType()
{
	UClass* DriverCls = ParentClass;
	if (DriverCls)
	{
		UObject* CDO = DriverCls->GetDefaultObject();
		ULogicFlowDriverInstance* DriverInstance = Cast<ULogicFlowDriverInstance>(CDO);
		if (DriverInstance)
		{
			return DriverInstance->DriverType;
		}
	}
	return EFlowDriverType::Default;
}

void ULogicFlowAsset::PostLoad()
{
	Super::PostLoad();
	FlowInstance->SetFlags(RF_Transactional | RF_ArchetypeObject);
}
