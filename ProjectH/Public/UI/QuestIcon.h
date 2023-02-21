// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Blueprint/UserWidget.h"
#include "QuestIcon.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UQuestIcon : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* DistanceText;

	int32 Distance;

public:
	virtual void NativeConstruct() override;
	void Init(float Dist);
	
};
