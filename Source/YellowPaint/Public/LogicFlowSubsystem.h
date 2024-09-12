// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Subsystems/WorldSubsystem.h"
#include "LogicFlowAsset.h"
#include "LogicFlowSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class YELLOWPAINT_API ULogicFlowSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	// Begin USubsystem
	/*virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;*/
	// End USubsystem

	UFUNCTION(BlueprintCallable, Category = "Logic FLow")
	ULogicFlowDriverInstance* CreateLogicFLow(ULogicFlowAsset* FlowAsset);

	
	UFUNCTION(BlueprintCallable, Category = "Logic FLow")
	void RunLogicFlow(ULogicFlowDriverInstance* FlowDriver);

protected:
	UFUNCTION(BlueprintCallable, Category = "Logic FLow")
	ULogicFlowDriverInstance* CreateLogicFlowDriverInstance(ULogicFlowDriverInstance* FlowDriver);
};
