#pragma once


#include "CoreMinimal.h"
#include "KismetCompiler.h"
#include "KismetCompilerModule.h"


class ULogicFlowAsset;
class UYellowPaintGeneratedClass;

/*
 * 无效 注册没有效果
 */
class YELLOWPAINTEDITOR_API FYellowPaintCompiler: public IBlueprintCompiler
{
public:
	FYellowPaintCompiler();

	bool CanCompile(const UBlueprint* Blueprint) override;
	void PreCompile(UBlueprint* Blueprint, const FKismetCompilerOptions& CompileOptions) override;
	void Compile(UBlueprint* Blueprint, const FKismetCompilerOptions& CompileOptions, FCompilerResultsLog& Results) override;
	void PostCompile(UBlueprint* Blueprint, const FKismetCompilerOptions& CompileOptions) override;
	virtual bool GetBlueprintTypesForClass(UClass* ParentClass, UClass*& OutBlueprintClass, UClass*& OutBlueprintGeneratedClass) const override;

	int32 CompileCount;

	
};


class YELLOWPAINTEDITOR_API FYellowPaintCompilerContext : public FKismetCompilerContext
{
public:

	FYellowPaintCompilerContext(ULogicFlowAsset* SourceBP, FCompilerResultsLog& InMessageLog, const FKismetCompilerOptions& InCompilerOptions);
	virtual ~FYellowPaintCompilerContext();

protected:
	
	typedef FKismetCompilerContext Super;

	// FKismetCompilerContext
	//virtual void CreateFunctionList() override;
	virtual void SpawnNewClass(const FString& NewClassName) override;
	virtual void OnNewClassSet(UBlueprintGeneratedClass* ClassToUse) override;
	virtual void PrecompileFunction(FKismetFunctionContext& Context, EInternalCompilerFlags InternalFlags) override;
	/*virtual void CleanAndSanitizeClass(UBlueprintGeneratedClass* ClassToClean, UObject*& InOutOldCDO) override;*/
	virtual void SaveSubObjectsFromCleanAndSanitizeClass(FSubobjectCollection& SubObjectsToSave, UBlueprintGeneratedClass* ClassToClean) override;
	virtual void EnsureProperGeneratedClass(UClass*& TargetClass) override;
	virtual void CopyTermDefaultsToDefaultObject(UObject* DefaultObject);
	virtual void FinishCompilingClass(UClass* Class) override;
	virtual bool ValidateGeneratedClass(UBlueprintGeneratedClass* Class) override;
	virtual bool IsNodePure(const UEdGraphNode* Node) const;

	/*void CompileQuestTemplate(ULogicFlowAsset* QuestBP, UObject* InParent);*/
	
	// End FKismetCompilerContext
	ULogicFlowAsset* GetFlowAsset() const;
protected:

	class UYellowPaintGeneratedClass* NewBlueprintClass;

	/** Compiler customization for Widgets */
	/*class UQuestGraphSchema* QuestSchema;*/
private:
	/*void CheckIfPropertyContainsPIE(UObject* ToCheckObj, const UEdGraphNode* ReportNode, FString ComponentName="");*/

};
