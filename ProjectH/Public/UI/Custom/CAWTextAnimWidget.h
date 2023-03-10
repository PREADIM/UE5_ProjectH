// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "UI/Custom/CustomAnimWidget.h"
#include "CAWTextAnimWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UCAWTextAnimWidget : public UCustomAnimWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* CAWText;

public:
	void SetCAWText(FString Text);
	
};
