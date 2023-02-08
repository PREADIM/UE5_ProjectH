// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Tema/ARPG/Widget/ARPGWidgetBase.h"
#include "ARPGWidget_State.generated.h"

/**
 * c
 */
UCLASS(Meta = (DisableNativeTick))
class PROJECTH_API UARPGWidget_State : public UARPGWidgetBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar* HP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar* PrevHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar* AP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar* PrevAP;

public:
	virtual void NativeConstruct() override;
	virtual void Init(class AARPGUnitBase* Unit) override;


public:
	UFUNCTION()
		void SetHP();

	void SetPrevHP();
	
	UFUNCTION(BlueprintCallable)
		float RetAP();


public:
	FTimerHandle DmgRenderTimer;
	FTimerHandle PrevHPHandle;

	// �� ���Լ��� �ε巴�� HP�� ����ϱ����� ����.
	float CurrentHPPercent;
	float LerpHPPercent;
	
};
