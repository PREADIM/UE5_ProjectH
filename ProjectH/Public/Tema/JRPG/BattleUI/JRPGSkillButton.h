// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Tema/JRPG/CustomWidget.h"
#include "JRPGSkillButton.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EButtonType : uint8
{
	NormalAttack UMETA(DisplayName = "Normal"),
	SkillAttack UMETA(DisplayName = "Skill")
};

UCLASS()
class PROJECTH_API UJRPGSkillButton : public UCustomWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
		class UButton* SkillButton;

	UPROPERTY(EditAnywhere)
		EButtonType ButtonType;

	UPROPERTY()
		class AJRPGUnit* CurrentUnit;
	int32 CostMP;


	UPROPERTY(EditAnywhere)
		FLinearColor NormalColor;
	UPROPERTY(EditAnywhere)
		FLinearColor HoveredColor;
	UPROPERTY(EditAnywhere)
		FLinearColor PressedColor;
	UPROPERTY(EditAnywhere)
		FLinearColor DisableColor;

public:
	void Init();
	void EnermyTurnFirstInit(); // ���� ��ó�� �����̶� ��¿������ �� ĳ������ ���� ����ĳ������ ������ �ʱ�ȭ.
	void SkillSetActive(); // MP�� ������ ����� �Ҽ� �����Ƿ� ��Ȱ��ȭ �ؾ��ϱ� ����.
	void ButtonSetImage(FSlateBrush Icon);


	UFUNCTION()
		void UseSkill();

};
