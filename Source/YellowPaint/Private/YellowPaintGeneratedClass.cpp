#include "YellowPaintGeneratedClass.h"
#include "KismetCompiler.h"
#include "KismetCompilerModule.h"
#include "LogicFlowNode.h"


void UYellowPaintGeneratedClass::Link(FArchive& Ar, bool bRelinkExistingProperties)
{
	Super::Link(Ar, bRelinkExistingProperties);
}

void UYellowPaintGeneratedClass::PostLoad()
{
	Super::PostLoad();

	/*if (QuestTemplate)
	{
		// We don't want any of these flags to carry over from the WidgetBlueprint
		QuestTemplate->ClearFlags(RF_Public | RF_ArchetypeObject | RF_DefaultSubObject);
	}*/
}

void UYellowPaintGeneratedClass::PurgeClass(bool bRecompilingOnLoad)
{
	Super::PurgeClass(bRecompilingOnLoad);
}