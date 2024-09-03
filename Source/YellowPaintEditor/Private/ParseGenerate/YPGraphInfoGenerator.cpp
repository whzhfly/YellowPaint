#include "YPGraphInfoGenerator.h"
#include "Interfaces/IPluginManager.h"
#include "JsonConvert/BpJsonConvert.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UYellowPaintGraph.h"

void YPGraphInfoGenerator::Generate(const TArray<FAssetData> SelectedAssets)
{
	UE_LOG(LogTemp, Warning, TEXT("MyLog: Something went wrong!"));
	for (FAssetData assetData : SelectedAssets)
	{
		UObject* obj = assetData.GetAsset();
		UYellowPaintGraph* QuestBP = CastChecked<UYellowPaintGraph>(obj);
		auto QuestLogicGeneratedClass = QuestBP->GeneratedClass;
		/*UQuestLogicBase* QuestLogic = nullptr;
		if (QuestLogicGeneratedClass->IsChildOf(UQuestLogicBase::StaticClass()))
		{
			UObject* DefaultObject = QuestLogicGeneratedClass->GetDefaultObject();
			QuestLogic = Cast<UQuestLogicBase>(DefaultObject);
		}*/
		TArray<UEdGraph*> Graphs;
		QuestBP->GetAllGraphs(Graphs);

		/*for (auto QuestGraph: Graphs)
		{
			const FString RootPath = IPluginManager::Get().FindPlugin("YellowPaintEditor")->GetBaseDir() / TEXT("Resources");

            /*FString fullPath = GetCaPath(assetData.AssetName);
            FString DTPath = GetDTPath();#1#
            // 这么写不知道为啥会变成同步的
            // TSharedPtr<FMyRunnable> MyRunnable = MakeShared<FMyRunnable>(obj, fullPath);
            // TSharedPtr<FRunnableThread> MyThread = MakeShareable(FRunnableThread::Create(MyRunnable.Get(), fullPath.GetCharArray().GetData()));
            /*FObjectToPythonStr Converter = FObjectToPythonStr(QuestGraph, QuestLogic, fullPath, assetData.AssetName);#1#
            /*FFileHelper::SaveStringToFile(Converter.GetPythonStr(), *fullPath, FFileHelper::EEncodingOptions::ForceUTF8);
            FDataTableToPythonStr DTConverter = FDataTableToPythonStr();
            FFileHelper::SaveStringToFile(DTConverter.GetPythonStr(), *DTPath, FFileHelper::EEncodingOptions::ForceUTF8);
            UE_LOG(LogTemp, Warning, TEXT("Save to: %s \n DT: %s"), *fullPath, *DTPath);#1# 
			
		}*/

		const FString RootPath = IPluginManager::Get().FindPlugin("YellowPaint")->GetBaseDir() / TEXT("Resources");


		if (!QuestBP) return;

		
		FString FileName = RootPath + QuestBP->GetName();
		
		auto jsonConverter = BpJsonConvert(QuestBP);
		FString jsonStr = jsonConverter.ConvertToStr();
		FString BPPath = UKismetSystemLibrary::GetSystemPath(QuestBP);
		FFileHelper::SaveStringToFile(jsonStr, *FileName, FFileHelper::EEncodingOptions::ForceUTF8WithoutBOM);
		UE_LOG(LogTemp, Log, TEXT("Export %s To JSON %s"), *BPPath, *FileName);
		
	}
}