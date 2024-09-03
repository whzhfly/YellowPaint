#include "YellowPaintGraphEditorCommands.h"


#define LOCTEXT_NAMESPACE "FYellowPaintGraphEditorCommands"

TSharedRef<FUICommandList> FYellowPaintGraphEditorCommands::ActionList(new FUICommandList());

void FYellowPaintGraphEditorCommands::RegisterCommands()
{
	UI_COMMAND(ExportJSONCommand, "Export JSON", "Export JSON", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(ShowDetails, "Details", "Show the details for this Quest.", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE