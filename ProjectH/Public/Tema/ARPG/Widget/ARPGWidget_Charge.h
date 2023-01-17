// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Tema/ARPG/Widget/ARPGWidgetBase.h"
#include "ARPGWidget_Charge.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UARPGWidget_Charge : public UARPGWidgetBase
{
	GENERATED_BODY()

public:
	virtual void Init(class AARPGUnitBase* Unit) override;
	virtual void NativeConstruct() override;


public:
	void BindCharge(float Ratio);


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar* Charge;

	
};
