// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaletteNode.h"
#include "YPBranchNode.generated.h"

/**
 * 
 */
UCLASS()
class YELLOWPAINT_API UYPBranchNode : public UPaletteNode
{
	GENERATED_BODY()

protected:
	void OnEnter_Implementation() override;
	
	void OnExit_Implementation() override;
};



UCLASS()
class YELLOWPAINT_API UYAsyncNode : public UPaletteNode
{
	GENERATED_BODY()

protected:
	void OnEnter_Implementation() override;
	
	void OnExit_Implementation() override;
};
