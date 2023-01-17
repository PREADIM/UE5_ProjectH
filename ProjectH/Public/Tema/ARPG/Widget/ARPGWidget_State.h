// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Tema/ARPG/Widget/ARPGWidgetBase.h"
#include "ARPGWidget_State.generated.h"

/**
 * c
 */
UCLASS()
class PROJECTH_API UARPGWidget_State : public UARPGWidgetBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar* HP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar* AP;

public:
	virtual void NativeConstruct() override;
	virtual void Init(class AARPGUnitBase* Unit) override;


public:
	UFUNCTION(BlueprintCallable)
		float RetHP();
	
	UFUNCTION(BlueprintCallable)
		float RetAP();
	
};
