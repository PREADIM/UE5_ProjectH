// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Blueprint/UserWidget.h"
#include "CustomAnimWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UCustomAnimWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
		void PlayCustomAnimation();	
};
