// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "Preferences/UnrealEdOptions.h"
#include "YellowPaintEditorFactory.generated.h"

/**
 * 
 */
UCLASS()
class YELLOWPAINTEDITOR_API UYellowPaintEditorFactory : public UFactory
{
	GENERATED_BODY()

	UYellowPaintEditorFactory();

	// UFactory interface
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext) override;
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;

	virtual bool ConfigureProperties() override;
	virtual bool CanCreateNew() const override;
	virtual FString GetDefaultNewAssetName() const override;
	virtual FText GetDisplayName() const override;

	// The parent class of the created blueprint
	UPROPERTY(EditAnywhere, Category = BlueprintFactory, meta = (AllowAbstract = "", BlueprintBaseOnly = ""))
	TSubclassOf<class UObject> ParentClass; // select click 

	UPROPERTY()
	TEnumAsByte<EBlueprintType> BlueprintType = EBlueprintType::BPTYPE_Normal;

public:
	static const TArray<FClassPickerDefaults>& GetPickerDefaultClasses();
	
};
