// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <BlueprintEditor.h>
/*#include "FYellowPaintGraphEditor.generated.h"*/

/**
 * 
 */

class YELLOWPAINTEDITOR_API FYellowPaintGraphEditor : public FBlueprintEditor
{
public:
	void InitBlueprintEditorExt(const EToolkitMode::Type Mode, const TSharedPtr< class IToolkitHost >& InitToolkitHost, const TArray<class UBlueprint*>& InBlueprints, bool bShouldOpenInDefaultsMode, UClass* InSchemaClass);


	virtual bool CanAddNewLocalVariable() const override { return false; }

	virtual bool AreMacrosAllowed() const override { return false; }
	
	virtual FName GetToolkitFName() const override { return FName("YellowPaintEditor"); }


	virtual UToolMenu* RegisterModeToolbarIfUnregistered(const FName InModeName) override;

protected:
	virtual void CreateDefaultCommands() override;

	virtual FGraphAppearanceInfo GetGraphAppearance(class UEdGraph* InGraph) const override;



	// User Define ========================================

	virtual void AddBlueprintLGOtherToolBar(UToolMenu* InMenu);
	
	/** Called when Compile button is clicked */
    virtual void ExportJSON();

	virtual void ShowDeatil();
};
