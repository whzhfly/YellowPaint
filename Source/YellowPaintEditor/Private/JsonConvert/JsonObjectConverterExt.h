// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JsonUtilities.h"

class FJsonObjectConverterExt : public FJsonObjectConverter
{
public:
	/* * Converts from a FProperty to a Json Value using exportText
	 *
	 * @param Property			The property to export
	 * @param Value				Pointer to the value of the property
	 * @param CheckFlags		Only convert properties that match at least one of these flags. If 0 check all properties.
	 * @param SkipFlags			Skip properties that match any of these flags
	 * @param ExportCb Optional callback to override export behavior, if this returns null it will fallback to the default
	 * @param OuterProperty		If applicable, the Array/Set/Map Property that contains this property
	 *
	 * @return					The constructed JsonValue from the property
	 */
	static TSharedPtr<FJsonValue> UPropertyToJsonValue(FProperty* Property, const void* Value, int64 CheckFlags = 0, int64 SkipFlags = 0, const CustomExportCallback* ExportCb = nullptr,
													   FProperty* OuterProperty = nullptr);

	/**
	 * Converts from a UStruct to a Json Object, using exportText
	 *
	 * @param StructDefinition UStruct definition that is looked over for properties
	 * @param Struct The UStruct instance to copy out of
	 * @param JsonObject Json Object to be filled in with data from the ustruct
	 * @param CheckFlags Only convert properties that match at least one of these flags. If 0 check all properties.
	 * @param SkipFlags Skip properties that match any of these flags
	 * @param ExportCb Optional callback to override export behavior, if this returns null it will fallback to the default
	 *
	 * @return False if any properties failed to write
	 */
	static bool UStructToJsonObject(const UStruct* StructDefinition, const void* Struct, TSharedRef<FJsonObject> OutJsonObject, int64 CheckFlags = 0, int64 SkipFlags = 0,
									const CustomExportCallback* ExportCb = nullptr);

	/**
	 * Converts from a UStruct to a set of json attributes (possibly from within a JsonObject)
	 *
	 * @param StructDefinition UStruct definition that is looked over for properties
	 * @param Struct The UStruct instance to copy out of
	 * @param OutJsonAttributes Map of attributes to copy in to
	 * @param CheckFlags Only convert properties that match at least one of these flags. If 0 check all properties.
	 * @param SkipFlags Skip properties that match any of these flags
	 * @param ExportCb Optional callback to override export behavior, if this returns null it will fallback to the default
	 *
	 * @return False if any properties failed to write
	 */
	static bool UStructToJsonAttributes(const UStruct* StructDefinition, const void* Struct, TMap<FString, TSharedPtr<FJsonValue> >& OutJsonAttributes, int64 CheckFlags = 0, int64 SkipFlags = 0,
										const CustomExportCallback* ExportCb = nullptr);
};
