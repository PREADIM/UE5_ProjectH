// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Blueprint/UserWidget.h"
#include "Tema/JRPG/JRPGUnitUIStruct.h"
#include "JRPGBattleWidget.generated.h"

/**
 * 
 */
// ★★ 배틀 상태 일때의 총 책임자 UI
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
			우선순위에 따른 턴
	-------------------------------*/

	UPROPERTY(meta = (BindWidget))
		class UJRPGPriority* JRPGPriorityList;

	/*--------------------
			스킬 버튼
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
			선택 가능한 적 리스트
	-------------------------------*/

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UHorizontalBox* EnermyList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UEnermyIconButton> BP_EnermyIcon;
	UPROPERTY()
		TArray<UEnermyIconButton*> Buttons; 

	/*--------------
		 락온 UI
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
			초기 설정
	-----------------------*/

	void Init();
	void PlayPriority(); // 배틀이 시작되고 위젯 실행.
	void SetUnitList(); // 게임할때 Hidden 하고 다시 Visible 하면 애초에 바뀐상태로 UnitList가 되어있음.
	void EnermyListInit(); // 적 리스트 초기화.

	/*-----------------------------------
			배틀 시 유동적인 위젯 설정
	-------------------------------------*/

	void PrioirtyBuffSetup(class AJRPGUnit* Unit);
	void SetVisible(bool bFlag);
	void SkillAndListButtonHidden(bool bFlag); // 적 차례시 위젯 단순화


	/*--------------------
			Lock On
	-----------------------*/

	void SetLockOn(int32 Num = 0); // 락온 UI 띄우기 함수.
	void VisibleLockOn();
	void HiddenLockOn();


	// 배틀 턴 시작
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
