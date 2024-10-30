// Fill out your copyright notice in the Description page of Project Settings.


#include "Asset/YellowPaintEditorFactory.h"
#include "LogicFlowAsset.h"
#include "Kismet2/SClassPickerDialog.h"
#include "ClassViewerFilter.h"
#include "ClassViewerModule.h"
#include "UnrealEdGlobals.h"
#include "LogicFlowDriverInstance.h"
#include "Preferences/UnrealEdOptions.h"
#include "Engine/BlueprintGeneratedClass.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "YellowPaintGeneratedClass.h"


#define LOCTEXT_NAMESPACE "YellowPaintEditorFactory"



class FLGClassFilter : public IClassViewerFilter
{
public:
	/** Classes to not allow any children of into the Class Viewer/Picker. */
	TSet< const UClass* > AllowedChildrenOfClasses;

	virtual void AutoAdjust()
	{
		TSet< const UClass* > NewAllowedChildrenOfClasses;
		for (auto cls : AllowedChildrenOfClasses)
		{
			ULogicFlowDriverInstance* CDO = cls->GetDefaultObject<ULogicFlowDriverInstance>();
			if (CDO->TemplateFlag)
			{
				NewAllowedChildrenOfClasses.Add(cls);
			}
		}
		AllowedChildrenOfClasses = NewAllowedChildrenOfClasses;
	}

	virtual bool IsClassAllowed(const FClassViewerInitializationOptions& InInitOptions, const UClass* InClass, TSharedRef< FClassViewerFilterFuncs > InFilterFuncs) override
	{
		if (InClass != nullptr)
		{
			for (auto cls : AllowedChildrenOfClasses)
			{
				if (InClass->IsChildOf(cls))
				{
					ULogicFlowDriverInstance* CDO = InClass->GetDefaultObject<ULogicFlowDriverInstance>();
					if (CDO->TemplateFlag)
					{
						return true;
					}
					else
					{
						return false;
					}
				}
			}
		}
		if (const UBlueprintGeneratedClass* Cls = Cast<UBlueprintGeneratedClass>(InClass))
		{
			UClass* SuperClass = Cls->GetSuperClass();
			if (SuperClass)
			{
				return IsClassAllowed(InInitOptions, SuperClass, InFilterFuncs);
			}
		}
		return InFilterFuncs->IfInChildOfClassesSet(AllowedChildrenOfClasses, InClass) == EFilterReturn::Passed && !InClass->HasAnyClassFlags(CLASS_Deprecated);
	}

	virtual bool IsUnloadedClassAllowed(const FClassViewerInitializationOptions& InInitOptions, const TSharedRef< const IUnloadedBlueprintData > InUnloadedClassData, TSharedRef< FClassViewerFilterFuncs > InFilterFuncs) override
	{
		return InFilterFuncs->IfInChildOfClassesSet(AllowedChildrenOfClasses, InUnloadedClassData) == EFilterReturn::Passed && !InUnloadedClassData->HasAnyClassFlags(CLASS_Deprecated);
	}
};


UYellowPaintEditorFactory::UYellowPaintEditorFactory()
{
	SupportedClass = ULogicFlowAsset::StaticClass();
	ParentClass = ULogicFlowDriverInstance::StaticClass();
	SelectDriverClass = ULogicFlowDriverInstance::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}


UObject* UYellowPaintEditorFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext)
{
	// ULogicFlowAsset ->
	if (!SelectDriverClass)
	{
		return nullptr;
	}
	check(Class->IsChildOf(ULogicFlowAsset::StaticClass()));
	auto ypGraphBP = CastChecked<ULogicFlowAsset>(FKismetEditorUtilities::CreateBlueprint(SelectDriverClass, InParent, Name, BPTYPE_Normal, ULogicFlowAsset::StaticClass(), UYellowPaintGeneratedClass::StaticClass(), CallingContext));
	return ypGraphBP;
}

UObject* UYellowPaintEditorFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return FactoryCreateNew(Class, InParent, Name, Flags, Context, Warn, NAME_None);
}

bool UYellowPaintEditorFactory::ConfigureProperties()
{
	/*return  true;*/
	// Null the parent class to ensure one is selected
	SelectDriverClass = nullptr;

	// Fill in options
	FClassViewerInitializationOptions Options;
	Options.Mode = EClassViewerMode::ClassPicker;
	Options.DisplayMode = EClassViewerDisplayMode::TreeView;
	Options.bShowObjectRootClass = true;

	// Only want blueprint actor base classes.
	Options.bIsBlueprintBaseOnly = false;

	// This will allow unloaded blueprints to be shown.
	Options.bShowUnloadedBlueprints = true;

	// Enable Class Dynamic Loading
	Options.bEnableClassDynamicLoading = true;

	Options.NameTypeToDisplay = EClassViewerNameTypeToDisplay::Dynamic;


	// Prevent creating blueprints of classes that require special setup (they'll be allowed in the corresponding factories / via other means)
	TSharedPtr<FLGClassFilter> Filter = MakeShareable(new FLGClassFilter);
	Filter->AllowedChildrenOfClasses.Add(ParentClass);
	Filter->AutoAdjust();
	
	// option add
	Options.ClassFilters.Add(Filter.ToSharedRef());
	
	const FText TitleText = LOCTEXT("CreateBlueprintOptions", "Pick Parent Class");
	UClass* ChosenClass = nullptr;
	const bool bPressedOk = SClassPickerDialog::PickClass(TitleText, Options, ChosenClass, UBlueprint::StaticClass());
	if (bPressedOk)
	{
		SelectDriverClass = ChosenClass;

		FEditorDelegates::OnFinishPickingBlueprintClass.Broadcast(SelectDriverClass);
	}

	return bPressedOk;
}

const TArray<FClassPickerDefaults>& UYellowPaintEditorFactory::GetPickerDefaultClasses()
{
	static TArray<FClassPickerDefaults> PickerDefaultClasses;
	PickerDefaultClasses.Empty();
	TArray<UClass*> Subclasses;
	GetDerivedClasses(UObject::StaticClass(), Subclasses, true);
	for (UClass* SubCls : Subclasses)
	{
		if (SubCls->HasMetaData("MOS_COMMON_PICK"))
		{
			FClassPickerDefaults PickerDefault(
				SubCls->GetPathName(),
				UBlueprint::StaticClass()->GetPathName()
			);
			PickerDefaultClasses.Add(PickerDefault);
		}
	}
	return PickerDefaultClasses;
}

bool UYellowPaintEditorFactory::CanCreateNew() const
{
	return true;
}

FString UYellowPaintEditorFactory::GetDefaultNewAssetName() const
{
	return FString(TEXT("NewLogicFlowAsset"));
}

FText UYellowPaintEditorFactory::GetDisplayName() const
{
	return LOCTEXT("AbilityDriver", "LogicFlow Asset");
}

// ================================= Child Override ================================

USkillEditorFactory::USkillEditorFactory():
Super()
{
	ParentClass = ULogicSkillFlowDriver::StaticClass();
	SelectDriverClass = ULogicSkillFlowDriver::StaticClass();
}


FText USkillEditorFactory::GetDisplayName() const
{
	return LOCTEXT("AbilityDriver", "Skill Asset");
}

UBuffEditorFactory::UBuffEditorFactory():
Super()
{
	ParentClass = ULogicBuffFlowDriver::StaticClass();
	SelectDriverClass = ULogicBuffFlowDriver::StaticClass();
}

FText UBuffEditorFactory::GetDisplayName() const
{
	return LOCTEXT("AbilityDriver", "Buff Asset");
}

UTriggerEditorFactory::UTriggerEditorFactory():
Super()
{
	ParentClass = ULogicTriggerFlowDriver::StaticClass();
	SelectDriverClass = ULogicTriggerFlowDriver::StaticClass();
}

FText UTriggerEditorFactory::GetDisplayName() const
{
	return LOCTEXT("AbilityDriver", "Trigger Asset");
}

#undef LOCTEXT_NAMESPACE