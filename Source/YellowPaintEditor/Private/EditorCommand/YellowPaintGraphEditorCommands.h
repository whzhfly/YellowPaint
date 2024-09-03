#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"

/**
 *
 */
class FYellowPaintGraphEditorCommands : public TCommands<FYellowPaintGraphEditorCommands>
{
public:
	static TSharedRef<FUICommandList> ActionList;

public:
	TSharedPtr<FUICommandInfo> ExportJSONCommand;
	TSharedPtr<FUICommandInfo> ShowDetails;

public:
	FYellowPaintGraphEditorCommands()
		: TCommands<FYellowPaintGraphEditorCommands>(
			  TEXT("YpGraph"), NSLOCTEXT("Contexts", "FYpGraph", "FYpGraph Plugin"), NAME_None, FAppStyle::GetAppStyleSetName()
		  )
	{
	}
	// TCommands<> interface
	virtual void RegisterCommands() override;
};
