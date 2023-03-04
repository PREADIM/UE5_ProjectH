// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Tema/ARPG/Widget/ARPGWidgetBase.h"
#include "ARPGWidget_BossHPView.generated.h"

/**
 * 
 */
UCLASS(Meta = (DisableNativeTick))
class PROJECTH_API UARPGWidget_BossHPView : public UARPGWidgetBase
{
	GENERATED_BODY()

public:
	virtual void Init(class AARPGUnitBase* Unit) override;
	//virtual void NativeConstruct() override;

public:
	UFUNCTION()
		void SetHP();
	void SetPrevHP();


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
		class UProgressBar* BossHP_Prev;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* DamageText;

	FTimerHandle DmgRenderTimer;
	FTimerHandle PrevHPHandle;

	UPROPERTY(EditAnywhere)
		float MaxDamageRenderDelay;
	float PrevDamage;
	float Delay;

	// 이 두함수는 부드럽게 HP를 계산하기위한 변수.
	float CurrentHPPercent;
	float LerpHPPercent;
};
