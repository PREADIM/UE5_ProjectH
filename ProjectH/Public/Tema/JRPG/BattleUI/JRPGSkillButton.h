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
	void EnermyTurnFirstInit(); // 적이 맨처음 사작이라 어쩔수없이 내 캐릭터중 가장 빠른캐릭터의 것으로 초기화.
	void SkillSetActive(); // MP가 적으면 사용을 할수 없으므로 비활성화 해야하기 때문.
	void ButtonSetImage(FSlateBrush Icon);


	UFUNCTION()
		void UseSkill();

};
