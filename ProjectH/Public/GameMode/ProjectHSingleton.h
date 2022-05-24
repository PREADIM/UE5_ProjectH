// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "UObject/NoExportTypes.h"
#include "ProjectHSingleton.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class PROJECTH_API UProjectHSingleton : public UObject
{
	GENERATED_BODY()

public:
	UProjectHSingleton();


	static UProjectHSingleton* GetSingleton();
	
};
