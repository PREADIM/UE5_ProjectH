// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Tema/ARPG/Widget/ARPGWidgetBase.h"
#include "ARPGWidget_BossHPView.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UARPGWidget_BossHPView : public UARPGWidgetBase
{
	GENERATED_BODY()

public:
	virtual void Init(class AARPGUnitBase* Unit) override;
	virtual void NativeConstruct() override;

public:
	UFUNCTION(BlueprintCallable)
		float RetHP();

	UFUNCTION()
		void SetTextDamage(float TakeDamage);

	UFUNCTION()
		void RenderDamageText();

public:
	UPROPERTY(VisibleAnywhere)
		class AARPGEnermy* BossUnit;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar* BossHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* DamageText;

	FTimerHandle DmgRenderTimer;

	UPROPERTY(EditAnywhere)
		float MaxDamageRenderDelay;
	float PrevDamage;
	float Delay;
	
};
