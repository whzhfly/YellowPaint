// Fill out your copyright notice in the Description page of Project Settings.


#include "YellowPaintEditorFactory.h"
#include "UYellowPaintGraph.h"
#include "Kismet2/SClassPickerDialog.h"
#include "GameFramework/Actor.h"
#include "ClassViewerFilter.h"
#include "ClassViewerModule.h"
#include "UnrealEdGlobals.h"
#include "Preferences/UnrealEdOptions.h"
#include "Engine/BlueprintGeneratedClass.h"
#include "Kismet2/KismetEditorUtilities.h"


#define LOCTEXT_NAMESPACE "YellowPaintEditorFactory"



class FLGClassFilter : public IClassViewerFilter
{
public:
	/** Classes to not allow any children of into the Class Viewer/Picker. */
	TSet< const UClass* > AllowedChildrenOfClasses;

	virtual bool IsClassAllowed(const FClassViewerInitializationOptions& InInitOptions, const UClass* InClass, TSharedRef< FClassViewerFilterFuncs > InFilterFuncs) override
	{
		/*if (InClass->HasMetaData(TEXT("MOS_EXPORT")))
		{
			if (FKismetEditorUtilities::CanCreateBlueprintOfClass(InClass))
			{
				return true;
			}
		}*/
		if (InClass != nullptr)
		{
			for (auto cls : AllowedChildrenOfClasses)
			{
				if (InClass->IsChildOf(cls))
				{
					return  true;
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
	SupportedClass = UYellowPaintGraph::StaticClass();
	//ParentClass = ULogicGraphBlueprint::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* UYellowPaintEditorFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext)
{
	if (!ParentClass)
	{
		return nullptr;
	}
	check(Class->IsChildOf(UYellowPaintGraph::StaticClass()));

	auto ypGraphBP = CastChecked<UYellowPaintGraph>(FKismetEditorUtilities::CreateBlueprint(ParentClass, InParent, Name, BPTYPE_Normal, UYellowPaintGraph::StaticClass(), UBlueprintGeneratedClass::StaticClass(), CallingContext));

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
	ParentClass = nullptr;

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
	Filter->AllowedChildrenOfClasses.Add(AActor::StaticClass());
	/*Filter->AllowedChildrenOfClasses.Add(UYellowPaintGraph::StaticClass());*/


	// option add
	Options.ClassFilters.Add(Filter.ToSharedRef());


	
	//Filter->AllowedChildrenOfClasses.Add(AActor::StaticClass());
	
	/*if (FYellowPaintEditorModule* EditorModule = FModuleManager::GetModulePtr<FYellowPaintEditorModule>("LogicGraphEditor"))
	{
		for (auto Iter : EditorModule->ParentClasses)
		{
			Filter->AllowedChildrenOfClasses.Add(Iter);
		}
	}*/

	/*UUnrealEdOptions::FGetNewAssetDefaultClasses& Delegate = GUnrealEd->GetUnrealEdOptions()->OnGetNewAssetDefaultClasses();
	Delegate.BindStatic(&UYellowPaintEditorFactory::GetPickerDefaultClasses);*/
	/*Delegate.Unbind();*/

	const FText TitleText = LOCTEXT("CreateBlueprintOptions", "Pick Parent Class");
	UClass* ChosenClass = nullptr;
	const bool bPressedOk = SClassPickerDialog::PickClass(TitleText, Options, ChosenClass, UBlueprint::StaticClass());
	if (bPressedOk)
	{
		ParentClass = ChosenClass;

		FEditorDelegates::OnFinishPickingBlueprintClass.Broadcast(ParentClass);
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
	return FString(TEXT("DefaultAbility"));
}

FText UYellowPaintEditorFactory::GetDisplayName() const
{
	return LOCTEXT("AbilityDriver", "AbilityDriver Graph");
}


#undef LOCTEXT_NAMESPACE