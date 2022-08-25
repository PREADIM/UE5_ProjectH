// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Tema/JRPG/CustomWidget.h"
#include "JRPGULTButton.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UJRPGULTButton : public UCustomWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
		class UButton* ULTButton;
	UPROPERTY(meta = (BindWidget))
		class UProgressBar* PB_ULT;

	UPROPERTY()
		class AJRPGUnit* CurrentUnit;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float ULTGage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bULT; // 궁극기 사용가능 여부.

public:
	void Init();
	UFUNCTION()
		void UseSkill();
	void EnermyTurnFirstInit(); // 적이 맨처음 사작이라 어쩔수없이 내 캐릭터중 가장 빠른캐릭터의 것으로 초기화.

};
