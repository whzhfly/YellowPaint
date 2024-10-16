// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "EdGraph/EdGraphNode.h"
#include "LogicFlowNode.generated.h"

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
