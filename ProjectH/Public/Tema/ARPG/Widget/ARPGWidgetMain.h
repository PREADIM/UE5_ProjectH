// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Tema/ARPG/Widget/ARPGWidgetBase.h"
#include "ARPGWidgetMain.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UARPGWidgetMain : public UARPGWidgetBase
{
	GENERATED_BODY()

public:
	virtual void Init(class AARPGUnitBase* Unit) override;

public:
	//차지 어택 위젯비율 전달
	void SetChargeAttacking(float Ratio);
	void ChargeAttackInViewport(bool bFlag);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UARPGWidget_State* State;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UARPGWidget_Charge* ChargeAttack;
};
