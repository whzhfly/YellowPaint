
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AssetTypeActions_Base.h"


class FAssetTypeActions_YellowPaintAsset : public FAssetTypeActions_Base
{
public:
	FAssetTypeActions_YellowPaintAsset(uint32 InAssetCategory, UClass* InSchemaClass)
		: Category(InAssetCategory)
		, SchemaClass(InSchemaClass)
	{}

public:
	// IAssetTypeActions Implementation
	virtual FText GetName() const override { return NSLOCTEXT("AssetTypeActions", "FAssetTypeActions_YellowPaintAsset", "YP Ability"); }
	virtual FColor GetTypeColor() const override { return FColor(255, 234, 80); }
	virtual UClass* GetSupportedClass() const override;
	virtual void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>()) override;
	
	virtual uint32 GetCategories() override { return Category; }

protected:
	uint32 Category;
	UClass* SchemaClass;
	
};
