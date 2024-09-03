// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "PaletteNode.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class YELLOWPAINT_API UPaletteNode : public UObject
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
};
