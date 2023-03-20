// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Blueprint/UserWidget.h"
#include "QuestMainCanvasWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UQuestMainCanvasWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UCanvasPanel* Canvas;
	
};
