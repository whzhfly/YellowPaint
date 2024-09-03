#include "JsonConvert/NodeHandler/LGNodeHandler_Default.h"

bool LGNodeHandler_Default::ShouldExposeAction(FBlueprintActionFilter const& Filter, FBlueprintActionInfo& BlueprintAction)
{
	return false;
}