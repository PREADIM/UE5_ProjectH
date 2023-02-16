// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Tema/JRPG/CustomWidget.h"
#include "DamageWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UDamageWidget : public UCustomWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* DamageAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UTextBlock* DamageText;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float DamageAmount;


public:
	void Init(float Damage, FVector TargetLocation);
	void PlayAnim();

	
};
