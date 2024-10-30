// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "EdGraph/EdGraphNode.h"
#include "LogicFlowNode.generated.h"



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


/*
class EFlowDriverType;*/

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class YELLOWPAINT_API ULogicFlowNode : public UObject
{
	GENERATED_BODY()


public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Flow", meta = (MultiLine="true"), DisplayName = "节点描述")
	FText NodeDescription = FText::FromString(TEXT(""));

	UFUNCTION(BlueprintNativeEvent)
	void OnEnter();
	
	UFUNCTION(BlueprintNativeEvent)
	bool CheckAssetEnable(EFlowDriverType DriveAsset);
	
	UFUNCTION(BlueprintNativeEvent)
	void OnExit();

#if WITH_EDITOR
	// Pins OR Other
	UFUNCTION(BlueprintNativeEvent)
	FText GetNodeTitle();
	
	UFUNCTION(BlueprintNativeEvent)
	void CollectionAllocatePin();
	
	UFUNCTION(BlueprintCallable)
    void AddAllocatePin(EEdGraphPinDirection Dir, FName PinCategory, FName PinName);
	
	UFUNCTION(BlueprintCallable)
	void ReAutoGeneratePins();

	UFUNCTION(BlueprintNativeEvent)
	bool CanExtraAddPins();
	
	UFUNCTION(BlueprintNativeEvent)
	void ExtraAddPins();
	
	UFUNCTION(BlueprintNativeEvent)
	void RefreshPins();

	struct PinInfoStruct
	{
		EEdGraphPinDirection Dir;
		FName PinCategory;
		FName PinName;
	};
	
	TArray<PinInfoStruct> PinInfoArray;

#endif
};
