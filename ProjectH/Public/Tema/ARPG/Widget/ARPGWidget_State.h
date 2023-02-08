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
	
	// 평상시 AP출력
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


	// 데미지 출력을 위한 타이머
	FTimerHandle DmgRenderTimer;

	// 부드러운 위젯 연출을 위한 타이머 핸들
	FTimerHandle PrevHPHandle;
	FTimerHandle PrevAPHandle;

	// 이 두 변수는 부드럽게 HP를 계산하기위한 변수.
	float CurrentHPPercent;
	float LerpHPPercent;

	// 이 두 변수는 부드럽게 AP를 계산하기위한 변수.
	float CurrentAPPercent;
	float LerpAPPercent;
	
};
