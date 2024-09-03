#pragma once



class YELLOWPAINTEDITOR_API BpJsonConvert
{
public:
	UBlueprint *bp_;

public:
	BpJsonConvert(UBlueprint *bp);
	TSharedPtr<FJsonObject> Convert();
	FString ConvertToStr();
	TSharedPtr<FJsonObject> GraphToJson(const UEdGraph *);
	TSharedPtr<FJsonObject> NodeToJson(const TObjectPtr<UEdGraphNode>);
	static TSharedPtr<FJsonValue> FJsonObjectConverterExportCB(FProperty *property, const void *value);
	static TSharedPtr<FJsonObject> PinToJson(const UEdGraphPin *);
	static TSharedPtr<FJsonValue> PinDefaultValueToJson(const UEdGraphPin *);
	// others
	static TSharedPtr<FJsonObject> PinTypeToJson(const FEdGraphPinType &pinType, bool no_container=false);
	static TSharedPtr<FJsonObject> FEdGraphTerminalTypeToJson(const FEdGraphTerminalType &tt);
	~BpJsonConvert();
};