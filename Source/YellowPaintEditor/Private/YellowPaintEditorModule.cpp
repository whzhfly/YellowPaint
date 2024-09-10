#include "YellowPaintEditorModule.h"
#include "AssetToolsModule.h"
#include "AssetToolsModule.h"
#include "Toolkits/AssetEditorToolkit.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include <ContentBrowserModule.h>
#include  "Asset/AssetTypeActions.h"
#include "Style/YellowPaintEditorStyle.h"
#include "Graph/Widgets//YellowPaintGraphNodeFactory.h"
// #include "BpToJson.h"

#define LOCTEXT_NAMESPACE "fYellowPaintEditorModule"

void FYellowPaintEditorModule::StartupModule()
{

	IAssetTools &AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	
	
	FKismetCompilerContext::RegisterCompilerForBP(ULogicFlowAsset::StaticClass(), [](UBlueprint* InBlueprint, FCompilerResultsLog& InMessageLog, const FKismetCompilerOptions& InCompileOptions)
	{
		return MakeShared<FYellowPaintCompilerContext>(CastChecked<ULogicFlowAsset>(InBlueprint), InMessageLog, InCompileOptions);
	});
	
	IKismetCompilerInterface& KismetCompilerModule = FModuleManager::LoadModuleChecked<IKismetCompilerInterface>("KismetCompiler");
	KismetCompilerModule.GetCompilers().Add(&YellowPaintCompiler);
	
	auto EditorCategory =
		AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("LogicFlow")), LOCTEXT("YellowPaintEditorCategory", "LogicFlow"));
	AssetTools.RegisterAssetTypeActions(MakeShared<FAssetTypeActions_YellowPaintAsset>(EditorCategory, UEdGraphSchema_K2::StaticClass()));

	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>(TEXT("ContentBrowser"));
	TArray<FContentBrowserMenuExtender_SelectedAssets>& MenuExtenderDelegates = ContentBrowserModule.GetAllAssetViewContextMenuExtenders();

	MenuExtenderDelegates.Add(FContentBrowserMenuExtender_SelectedAssets::CreateStatic(&FYellowPaintEditorModule::OnExtendSkelMeshWithDNASelectionMenu));


	YellowPaintNodeFactory = MakeShareable(new FYellowPaintGraphNodeFactory());
	FEdGraphUtilities::RegisterVisualNodeFactory(YellowPaintNodeFactory);

	YellowPaintPinFactory = MakeShareable(new FYellowPaintGraphPinFactory());
	FEdGraphUtilities::RegisterVisualPinFactory(YellowPaintPinFactory);
	


	FYellowPaintEditorStyle::Initialize();
}

void FYellowPaintEditorModule::ShutdownModule()
{
	FEdGraphUtilities::UnregisterVisualNodeFactory(YellowPaintNodeFactory);

	FEdGraphUtilities::UnregisterVisualPinFactory(YellowPaintPinFactory);

	FYellowPaintEditorStyle::Shutdown();
}

TSharedRef<FExtender> FYellowPaintEditorModule::OnExtendSkelMeshWithDNASelectionMenu(const TArray<FAssetData>& SelectedAssets)
{
	TArray<FAssetData> QuestAssets;
	TSharedRef<FExtender> Extender = MakeShared<FExtender>();

	// 类型筛选
	for (auto Asset : SelectedAssets)
	{
		if (Asset.GetClass()->IsChildOf(ULogicFlowAsset::StaticClass()))
		{
			QuestAssets.Add(Asset);
		}
	}
	if (QuestAssets.Num() > 0)
	{
		Extender->AddMenuExtension(
			"GetAssetActions",
			EExtensionHook::After,
			nullptr,
			FMenuExtensionDelegate::CreateStatic(&CreateDnaActionsSubMenu, QuestAssets)
		);
	}
	return Extender;
}

void FYellowPaintEditorModule::CreateDnaActionsSubMenu(FMenuBuilder& MenuBuilder, const TArray<FAssetData> SelectedAssets)
{
	/*MenuBuilder.AddMenuEntry(
		FText::FromString (TEXT("生成Python数据")),
		FText::FromString (TEXT("生成服务器Python数据")),
		FSlateIcon(FQuestEditorStyle::GetStyleSetName(), "QuestEditor.Graph.GeneratePython"),
		FUIAction(FExecuteAction::CreateStatic(QuestServerGenerator::Generate, SelectedAssets))
	);*/
}

void FYellowPaintEditorModule::CreateEditor(
	ULogicFlowAsset *Blueprint, TSharedPtr<class IToolkitHost> EditWithinLevelEditor, UClass *SchemaClass
)
{
	UClass *ParentCls = Blueprint->ParentClass;
	UClass *SCls = ParentCls;
	/*while (SCls && SCls != UObject::StaticClass() && !CreateEditorFuns.Contains(SCls))
	{
		SCls = SCls->GetSuperClass();
	}
	if (CreateEditorFuns.Contains(SCls))
	{
		auto Fun = CreateEditorFuns[SCls];
		TSharedPtr<FLogicGraphEditor> Editor = Fun(SchemaClass);
		if (Editor.IsValid())
		{
			TArray<UBlueprint *> InBlueprints;
			InBlueprints.Add(Blueprint);
			Editor->InitBlueprintEditorExt(EToolkitMode::Standalone, EditWithinLevelEditor, InBlueprints, false, SchemaClass);
		}
	}
	else*/
	{
		TSharedRef<FYellowPaintGraphEditor> NewEditor(new FYellowPaintGraphEditor());
		TArray<UBlueprint *> InBlueprints;
		InBlueprints.Add(Blueprint);
		NewEditor->InitBlueprintEditorExt(EToolkitMode::Standalone, EditWithinLevelEditor, InBlueprints, false, SchemaClass);
	}
}

#undef LOCTEXT_NAMESPACE
IMPLEMENT_MODULE(FYellowPaintEditorModule, YellowPaintEditor)
