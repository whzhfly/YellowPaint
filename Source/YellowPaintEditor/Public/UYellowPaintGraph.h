// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Blueprint.h"
#include "UYellowPaintGraph.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class YELLOWPAINTEDITOR_API UYellowPaintGraph : public UBlueprint
{
	GENERATED_UCLASS_BODY()
public:
	virtual bool SupportedByDefaultBlueprintFactory() const override{ return false; }
	virtual bool CanAlwaysRecompileWhilePlayingInEditor() const override { return true; }
	virtual bool IsValidForBytecodeOnlyRecompile() const override { return false; }

	static bool ValidateGeneratedClass(const UClass* InClass) { return true; }
	//Return true if there is already a state with the same name in the graph. Return a new name that is not identical by ref.
	bool DoesStateHaveDuplicate(const class UQuestState* State, FName& OutBetterName) { return false; }
	
	/**
	 * Returns true if this blueprint supports event graphs
	 */
	virtual bool SupportsEventGraphs() const override { return false; }

	/**
	 * Returns true if this blueprint supports macros
	 */
	virtual bool SupportsMacros() const override { return false; }

	/**
	 * Returns true if this blueprint supports functions
	 */
	virtual bool SupportsFunctions() const override { return false; }

	/**
	 * Returns true if this blueprint supports delegates
	 */
	virtual bool SupportsDelegates() const override { return false; }
};
