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
		bool bULT; // �ñر� ��밡�� ����.

public:
	void Init();
	UFUNCTION()
		void UseSkill();
	void EnermyTurnFirstInit(); // ���� ��ó�� �����̶� ��¿������ �� ĳ������ ���� ����ĳ������ ������ �ʱ�ȭ.

};
