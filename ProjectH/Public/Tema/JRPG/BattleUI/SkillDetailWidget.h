// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Blueprint/UserWidget.h"
#include "SkillDetailWidget.generated.h"

/**
 * 
 */
UCLASS(Meta = (DisableNativeTick))
class PROJECTH_API USkillDetailWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* AttackName;
	UPROPERTY(meta = (BindWidget))
		class URichTextBlock* RichAttackDetail;

public:
	void Init(FString Name, FString Detail);
	
};
