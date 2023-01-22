// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Tema/ARPG/Widget/ARPGWidgetBase.h"
#include "ARPGWidget_BattleHP.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UARPGWidget_BattleHP : public UARPGWidgetBase
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

	void RenderDamageText(); // 데미지가 일정시간이 지나면 알아서 숫자가 사라지게 한다.

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar* BattleHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* Damage;

	FTimerHandle DmgRenderTimer;

	UPROPERTY(EditAnywhere)
		float MaxDamageRenderDelay; // 데미지가 언제까지 뜨게 할 것인가?
	UPROPERTY(EditAnywhere)
		float MaxRenderDelay; // 데미지가 언제까지 뜨게 할 것인가?
	float Delay;	

	// 소울류 처럼 데미지가 중첩되서 보이게 한다.
	float PrevDamage;
};
