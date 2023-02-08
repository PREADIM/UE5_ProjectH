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
	virtual void NativeConstruct() override;
	virtual void Init(class AARPGUnitBase* Unit) override;


public:
	UFUNCTION()
		void SetHP();
	void SetPrevHP();

	UFUNCTION()
		void SetAP();
	void SetPrevAP();
	
	// ���� AP���
	UFUNCTION(BlueprintCallable)
		float RetAP();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar* HP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar* PrevHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar* AP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar* PrevAP;


	// ������ ����� ���� Ÿ�̸�
	FTimerHandle DmgRenderTimer;

	// �ε巯�� ���� ������ ���� Ÿ�̸� �ڵ�
	FTimerHandle PrevHPHandle;
	FTimerHandle PrevAPHandle;

	// �� �� ������ �ε巴�� HP�� ����ϱ����� ����.
	float CurrentHPPercent;
	float LerpHPPercent;

	// �� �� ������ �ε巴�� AP�� ����ϱ����� ����.
	float CurrentAPPercent;
	float LerpAPPercent;
	
};
