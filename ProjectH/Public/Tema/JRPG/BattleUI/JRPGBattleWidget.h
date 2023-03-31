// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Blueprint/UserWidget.h"
#include "Tema/JRPG/JRPGUnitUIStruct.h"
#include "JRPGBattleWidget.generated.h"

/**
 * 
 */
// �ڡ� ��Ʋ ���� �϶��� �� å���� UI
UCLASS()
class PROJECTH_API UJRPGBattleWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere)
		class AJRPGPlayerController* OwnerController;

	UPROPERTY(VisibleAnywhere)
		class AJRPGGameMode* GM;

	/*-------------------------------
			�켱������ ���� ��
	-------------------------------*/

	UPROPERTY(meta = (BindWidget))
		class UJRPGPriority* JRPGPriorityList;

	/*--------------------
			��ų ��ư
	-----------------------*/

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UJRPGSkillButton* NormalAttack;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UJRPGSkillButton* SkillButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UJRPGULTButton* ULTButton;
	UPROPERTY(meta = (BindWidget))
		class USkillDetailWidget* DetailWidget;

	/*-------------------------------
			���� ������ �� ����Ʈ
	-------------------------------*/

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UHorizontalBox* EnermyList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UEnermyIconButton> BP_EnermyIcon;
	UPROPERTY()
		TArray<UEnermyIconButton*> Buttons; 

	/*--------------
		 ���� UI
	----------------*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class ULockOnWidget> BP_LockOnIcon;
	UPROPERTY()
		class ULockOnWidget* LockOnIcon;
	UPROPERTY()
		class UCanvasPanelSlot* CanvasSlot;

	FVector TargetLockOn;
	FVector2D Pos;
	int32 TargetNumber = 0;

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, const float InDeltaTime) override;

	/*--------------------
			�ʱ� ����
	-----------------------*/

	void Init();
	void PlayPriority(); // ��Ʋ�� ���۵ǰ� ���� ����.
	void SetUnitList(); // �����Ҷ� Hidden �ϰ� �ٽ� Visible �ϸ� ���ʿ� �ٲ���·� UnitList�� �Ǿ�����.
	void EnermyListInit(); // �� ����Ʈ �ʱ�ȭ.

	/*-----------------------------------
			��Ʋ �� �������� ���� ����
	-------------------------------------*/

	void PrioirtyBuffSetup(class AJRPGUnit* Unit);
	void SetVisible(bool bFlag);
	void SkillAndListButtonHidden(bool bFlag); // �� ���ʽ� ���� �ܼ�ȭ


	/*--------------------
			Lock On
	-----------------------*/

	void SetLockOn(int32 Num = 0); // ���� UI ���� �Լ�.
	void VisibleLockOn();
	void HiddenLockOn();


	// ��Ʋ �� ����
	void BattleTurnInit();


	/*------------------
		Skill Detail
	-------------------*/

	UPROPERTY()
		TArray<FAttackNameAndDetail> AttackDetail;

	UFUNCTION()
		void OnNormalAttaclDetail();
	UFUNCTION()
		void OnSkillAttackDetail();
	UFUNCTION()
		void OnULTAttackDetail();
	UFUNCTION()
		void OffSkillDetail();

};
