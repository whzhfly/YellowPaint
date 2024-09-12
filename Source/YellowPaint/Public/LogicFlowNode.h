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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "YP", meta = (MultiLine = "true"), DisplayName = "信息")
	FText NodeDescription = FText::FromString(TEXT(""));


	UFUNCTION(BlueprintNativeEvent)
	FText GetNodeTitle();

	UFUNCTION(BlueprintNativeEvent)
	void OnEnter();

	
	UFUNCTION(BlueprintNativeEvent)
	void OnExit();

#if WITH_EDITOR
	// Pins OR Other
	
	UFUNCTION(BlueprintNativeEvent)
	void CollectionAllocatePin();
	
	UFUNCTION(BlueprintCallable)
    void AddAllocatePin(EEdGraphPinDirection Dir, FName PinCategory, FName PinName);

	struct PinInfoStruct
	{
		EEdGraphPinDirection Dir;
		FName PinCategory;
		FName PinName;
	};
	
	TArray< PinInfoStruct> PinInfoArray;
#endif
};
