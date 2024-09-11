#include "Graph/Compile/YellowPaintCompiler.h"
#include "LogicFlowAsset.h"
#include "Kismet2/KismetReinstanceUtilities.h"
#include "LogicFlowDriverInstance.h"
#include "Graph/Nodes/EdYellowPaintNode.h"
#include "Graph/UEdYellowPaintGraph.h"
#include "YellowPaintGeneratedClass.h"

#define LOCTEXT_NAMESPACE "YellowPaintCompiler"

FYellowPaintCompiler::FYellowPaintCompiler()
{
	CompileCount = 0;
}

bool FYellowPaintCompiler::CanCompile(const UBlueprint* Blueprint)
{
	return Cast<ULogicFlowAsset>(Blueprint) != nullptr;
}

void FYellowPaintCompiler::PreCompile(UBlueprint* Blueprint, const FKismetCompilerOptions& CompileOptions)
{
}

void FYellowPaintCompiler::Compile(UBlueprint* Blueprint, const FKismetCompilerOptions& CompileOptions, FCompilerResultsLog& Results)
{
	if (ULogicFlowAsset* BlueprintAsset = CastChecked<ULogicFlowAsset>(Blueprint))
	{
		FYellowPaintCompilerContext Compiler(BlueprintAsset, Results, CompileOptions);
		Compiler.Compile();
		check(Compiler.NewClass);
	}
}

void FYellowPaintCompiler::PostCompile(UBlueprint* Blueprint, const FKismetCompilerOptions& CompileOptions)
{
	if (ULogicFlowAsset* LogicAsset = CastChecked<ULogicFlowAsset>(Blueprint))
	{
		UClass* GenerateClass = LogicAsset->GeneratedClass;
		UObject* AssetParentCDO = GenerateClass->GetDefaultObject();
		ULogicFlowDriverInstance* LogicDrive = CastChecked<ULogicFlowDriverInstance>(AssetParentCDO);
		/*if (LogicDrive)
		{
			DuplicateObject(LogicDrive, LogicAsset->FlowInstance);
		}*/
	}
}

bool FYellowPaintCompiler::GetBlueprintTypesForClass(UClass* ParentClass, UClass*& OutBlueprintClass, UClass*& OutBlueprintGeneratedClass) const
{
	if (ParentClass == ULogicFlowDriverInstance::StaticClass() || ParentClass->IsChildOf(ULogicFlowDriverInstance::StaticClass()))
	{
		OutBlueprintClass = ULogicFlowAsset::StaticClass();
		OutBlueprintGeneratedClass = UYellowPaintGeneratedClass::StaticClass();
		return true;
	}

	return false;
}


////////////////////////////////// Context //////////////////////////////////////////

FYellowPaintCompilerContext::FYellowPaintCompilerContext(ULogicFlowAsset* SourceBP, FCompilerResultsLog& InMessageLog, const FKismetCompilerOptions& InCompilerOptions)
	: Super(SourceBP, InMessageLog, InCompilerOptions)
	, NewBlueprintClass(nullptr)
{
	/*FString QuestAssetName = Blueprint->GetPathName() + "_C"; // 实际运行的蓝图名称中有_C后缀*/
	/*FNarrativeQuestEditorModule& QuestEditorModule = FModuleManager::GetModuleChecked<FNarrativeQuestEditorModule>("NarrativeQuestEditor");
	if (!QuestEditorModule.PlayerStateCache.IsValid()) return;
	UQuestComponent* QuestComp = QuestEditorModule.PlayerStateCache->FindComponentByClass<UQuestComponent>();
	if (!QuestComp) return;
	if (QuestComp->ActiveQuestLines.Contains(QuestAssetName)) {
		MessageLog.Error(*LOCTEXT("QuestCompileError", "Quest line @@ already active, please give up first").ToString(), *QuestAssetName);
	}*/
}

FYellowPaintCompilerContext::~FYellowPaintCompilerContext()
{
}

void FYellowPaintCompilerContext::SpawnNewClass(const FString& NewClassName)
{

	NewBlueprintClass = FindObject<UYellowPaintGeneratedClass>(Blueprint->GetOutermost(), *NewClassName);

	if (NewBlueprintClass == nullptr)
	{
		NewBlueprintClass = NewObject<UYellowPaintGeneratedClass>(Blueprint->GetOutermost(), FName(*NewClassName), RF_Public | RF_Transactional);
	}
	else
	{
		// Already existed, but wasn't linked in the Blueprint yet due to load ordering issues
		FBlueprintCompileReinstancer::Create(NewBlueprintClass);
	}
	NewClass = NewBlueprintClass;
}

void FYellowPaintCompilerContext::OnNewClassSet(UBlueprintGeneratedClass* ClassToUse)
{
	NewBlueprintClass = Cast<UYellowPaintGeneratedClass>(ClassToUse);
}

void FYellowPaintCompilerContext::PrecompileFunction(FKismetFunctionContext& Context, EInternalCompilerFlags InternalFlags)
{
	Super::PrecompileFunction(Context, InternalFlags);
}

void FYellowPaintCompilerContext::SaveSubObjectsFromCleanAndSanitizeClass(FSubobjectCollection& SubObjectsToSave, UBlueprintGeneratedClass* ClassToClean)
{
	Super::SaveSubObjectsFromCleanAndSanitizeClass(SubObjectsToSave, ClassToClean);

	check(ClassToClean == NewClass);
	NewBlueprintClass = Cast<UYellowPaintGeneratedClass>((UObject*)NewClass);

	ULogicFlowAsset* FlowAsset = GetFlowAsset();
	SubObjectsToSave.AddObject(FlowAsset->FlowInstance);
}

void FYellowPaintCompilerContext::EnsureProperGeneratedClass(UClass*& TargetUClass)
{
	if (TargetUClass && !((UObject*)TargetUClass)->IsA(UYellowPaintGeneratedClass::StaticClass()))
	{
		FKismetCompilerUtilities::ConsignToOblivion(TargetUClass, Blueprint->bIsRegeneratingOnLoad);
		TargetUClass = nullptr;
	}
}


void FYellowPaintCompilerContext::CopyTermDefaultsToDefaultObject(UObject* DefaultObject)
{
	FKismetCompilerContext::CopyTermDefaultsToDefaultObject(DefaultObject);
	ULogicFlowAsset* FlowAsset = GetFlowAsset();
	if (Cast<ULogicFlowDriverInstance>(DefaultObject))
	{
		if (FlowAsset != nullptr)
		{
			ULogicFlowDriverInstance* Instance = FlowAsset->FlowInstance;
			ULogicFlowDriverInstance* NewInstance = Cast<ULogicFlowDriverInstance>(DuplicateObject(DefaultObject, DefaultObject->GetOutermost()));
			Instance->DeepCopyFormAnother(NewInstance);
			/*
			if (NewInstance != nullptr)
			{
				FlowAsset->FlowInstance = NewInstance;
			}*/
		}
	}
}

ULogicFlowAsset* FYellowPaintCompilerContext::GetFlowAsset() const
{
	return Cast<ULogicFlowAsset>(Blueprint);
}


void FYellowPaintCompilerContext::CollectAllFlowNodes()
{
	ULogicFlowAsset* FlowAsset = GetFlowAsset();
	if (FlowAsset)
	{
		TArray<UEdGraph*> Graphs;
		FlowAsset->GetAllGraphs(Graphs);
		for (UEdGraph* Graph : Graphs)
		{
			UEdYellowPaintGraph* PaintGraph = Cast<UEdYellowPaintGraph>(Graph);
			if (PaintGraph)
			{
				for (auto Node: PaintGraph->Nodes)
				{
					UEdYellowPaintNode* PaintNode = Cast<UEdYellowPaintNode>(Node);
					if (PaintNode)
					{
						if (FlowAsset->FlowInstance)
						{
							// 是否复制 todo
							/*ULogicFlowNode* NewFlowNode = DuplicateObject(PaintNode->FlowNode, FlowAsset->GeneratedClass);
							 *这里的名字换了 但是这个新的NewFlowNode对象并没有 没有保存   保存的还是PaintNode->FlowNode 这个 这个是因为Graph Node FlowNode 自动保存的
							FlowAsset->FlowInstance->FlowNodesMap.Add(PaintNode->NodeGuid.ToString(), NewFlowNode);*/
						}
					}
				}
			}
		}
	}
}

void FYellowPaintCompilerContext::FinishCompilingClass(UClass* Class)
{
	ULogicFlowAsset* FlowAsset = GetFlowAsset();
	UYellowPaintGeneratedClass* GeneratedClass = Cast<UYellowPaintGeneratedClass>(Class);

	UClass* ParentClass = FlowAsset->ParentClass;
	const bool bIsSkeletonOnly = CompileOptions.CompileType == EKismetCompileType::SkeletonOnly;

	if (!bIsSkeletonOnly)
	{
		
		UObject* AssetParentCDO = GeneratedClass->GetDefaultObject();
		ULogicFlowDriverInstance* LogicDriver = CastChecked<ULogicFlowDriverInstance>(AssetParentCDO);
		if (FlowAsset->FlowInstance)
		{
			// set default
			// 这里编辑器的 没法设置属性到CDO 
			/*GeneratedClass->ClassDefaultObject = FlowAsset->FlowInstance;*/
			/*FlowAsset->FlowInstance = NewObject<ULogicFlowDriverInstance>(FlowAsset->GetOutermost());*/
		}
		if (LogicDriver != nullptr && FlowAsset->FlowInstance)
		{
			if (FlowAsset->FlowInstance.Get()->ExeCount == 0)
			{
				FlowAsset->FlowInstance.Get()->ExeCount = 100;
			}
			/*DuplicateObject(LogicDriver, FlowAsset->FlowInstance);*/

			GeneratedClass->FlowDriverInstance = FlowAsset->FlowInstance;
		}

		CollectAllFlowNodes();
		
		/*ULogicFlowDriverInstance* NewFlowDriverInstance = Cast<ULogicFlowDriverInstance>(StaticDuplicateObject(QuestBP->QuestTemplate, BPGClass, NAME_None, RF_AllFlags & ~RF_DefaultSubObject));
		BPGClass->SetQuestTemplate(NewQuestTemplate);*/
		
		if (!FlowAsset->bHasBeenRegenerated)
		{
			UBlueprint::ForceLoadMembers(FlowAsset->FlowInstance);
		}

		/*if (GeneratedClass && FlowAsset->FlowInstance && FlowAsset->QuestGraph)
		{
			// 这里不load的话 组件会找不到定义
			UBlueprint::ForceLoadMembers(QuestBP->QuestGraph);
			CompileQuestTemplate(QuestBP, BPGClass);
			EObjectFlags PreviousFlags = QuestBP->QuestTemplate->GetFlags();
			QuestBP->QuestTemplate->ClearFlags(RF_ArchetypeObject);

			UQuestLogicBase* NewQuestTemplate = Cast<UQuestLogicBase>(StaticDuplicateObject(QuestBP->QuestTemplate, BPGClass, NAME_None, RF_AllFlags & ~RF_DefaultSubObject));
			BPGClass->SetQuestTemplate(NewQuestTemplate);

			QuestBP->QuestTemplate->SetFlags(PreviousFlags);
		}*/

	}

	Super::FinishCompilingClass(Class);
}

bool FYellowPaintCompilerContext::ValidateGeneratedClass(UBlueprintGeneratedClass* Class)
{
	bool SuperResult = Super::ValidateGeneratedClass(Class);
	bool Result = ULogicFlowAsset::ValidateGeneratedClass(Class);

	return SuperResult && Result;
}


bool FYellowPaintCompilerContext::IsNodePure(const UEdGraphNode* Node) const
{
	if (const UK2Node* K2Node = Cast<const UK2Node>(Node))
	{
		return K2Node->IsNodePure();
	}
	// Uncomment this line from engine as it causes ensure on load which is annoying
	//ensure(Node->IsA(UEdGraphNode_Comment::StaticClass()) || Node->IsA(UEdGraphNode_Documentation::StaticClass()));
	return true;
}

#undef LOCTEXT_NAMESPACE
