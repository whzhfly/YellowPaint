#pragma once

#include <functional>
// #include "LogicGraphEditor.h"
#include "Asset/FYellowPaintGraphEditor.h"
#include "LogicFlowAsset.h"
#include "Modules/ModuleManager.h"
#include "Graph/Compile/YellowPaintCompiler.h"


class FYellowPaintGraphNodeFactory;

class FYellowPaintEditorModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;


	TSharedPtr<class FYellowPaintGraphNodeFactory> YellowPaintNodeFactory;

	TSharedPtr<class FYellowPaintGraphPinFactory> YellowPaintPinFactory;

	
public:
	static TSharedRef<FExtender> OnExtendSkelMeshWithDNASelectionMenu(const TArray<FAssetData>& SelectedAssets);
	static void CreateDnaActionsSubMenu(FMenuBuilder& MenuBuilder, const TArray<FAssetData> SelectedAssets);
	
	void CreateEditor(ULogicFlowAsset *Blueprint, TSharedPtr<class IToolkitHost> EditWithinLevelEditor, UClass *SchemaClass);

	FYellowPaintCompiler YellowPaintCompiler;
// 	virtual void RegisterLGEditor(UClass *ParentCls, TFunction<TSharedPtr<FLogicGraphEditor>(UClass *)> Handle);
// 	virtual void UnregisterLGEditor(UClass *ParentCls);

// 	virtual void RegisterParentCls(UClass *ParentCls) { ParentClasses.Add(ParentCls); }

// public:
// 	void CreateEditor(ULogicGraphBlueprint *Blueprint, TSharedPtr<class IToolkitHost> EditWithinLevelEditor, UClass *SchemaClass);

// private:
// 	void OnFEngineLoopInitComplete();
// 	void OnClickExportLGCppAction();

// 	void RegisterExporterMenus();

// public:
// 	TSet<UClass *> ParentClasses;

// private:
// 	TMap<UClass *, TFunction<TSharedPtr<FLogicGraphEditor>(UClass *)>> CreateEditorFuns;
// 	TSharedPtr<class FUICommandList> ExportLGCppAction;
// 	FDelegateHandle DelegateHandle;

// 	// ----- Content Browser Menu -----
// private:
// 	TSharedRef<FExtender> OnExtendContentBrowserAssetSelectionMenu(const TArray<FAssetData> &SelectedAssets);
// 	void CreateContentBrowserAssetMenu(FMenuBuilder &MenuBuilder, TArray<FAssetData> SelectedAssets);

// 	// ----- Hook API for user -----
// public:
// 	using ExportPathFunc = std::function<FString(UBlueprint *bp, const FAssetData &)>;
// 	virtual void SetExportPathFunc(ExportPathFunc func);
// 	virtual ExportPathFunc GetExportPathFunc();

// private:
// 	ExportPathFunc exportPathFunc = nullptr;
};
