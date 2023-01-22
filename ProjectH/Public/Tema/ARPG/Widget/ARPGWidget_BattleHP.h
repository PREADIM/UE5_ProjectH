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

	void RenderDamageText(); // �������� �����ð��� ������ �˾Ƽ� ���ڰ� ������� �Ѵ�.

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar* BattleHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* Damage;

	FTimerHandle DmgRenderTimer;

	UPROPERTY(EditAnywhere)
		float MaxDamageRenderDelay; // �������� �������� �߰� �� ���ΰ�?
	UPROPERTY(EditAnywhere)
		float MaxRenderDelay; // �������� �������� �߰� �� ���ΰ�?
	float Delay;	

	// �ҿ�� ó�� �������� ��ø�Ǽ� ���̰� �Ѵ�.
	float PrevDamage;
};
