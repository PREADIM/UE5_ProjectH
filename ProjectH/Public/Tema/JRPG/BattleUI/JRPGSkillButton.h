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

UCLASS(Meta = (DisableNativeTick))
class PROJECTH_API UJRPGSkillButton : public UCustomWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
		class UButton* SkillButton;

	UPROPERTY()
		class UJRPGBattleWidget* OwnerMainWidget;


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
	void SkillSetActive(); // MP�� ������ ����� �Ҽ� �����Ƿ� ��Ȱ��ȭ �ؾ��ϱ� ����.
	void ButtonSetImage(FSlateBrush Icon);
	void SetButton(UTexture2D* Tex);

	UFUNCTION()
		void UseSkill();

};
