// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Blueprint/UserWidget.h"
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

	UPROPERTY(meta = (BindWidget))
		class UJRPGPriority* JRPGPriorityList;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UJRPGSkillButton* NormalAttack;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UJRPGSkillButton* SkillButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UJRPGULTButton* ULTButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UHorizontalBox* EnermyList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UEnermyIconButton> BP_EnermyIcon;

	UPROPERTY()
		TArray<UEnermyIconButton*> Buttons; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class ULockOnWidget> BP_LockOnIcon;
	UPROPERTY()
		class ULockOnWidget* LockOnIcon;

	UPROPERTY()
		class UCanvasPanelSlot* CanvasSlot;

	FVector TargetLockOn;
	FVector2D Pos;


	UPROPERTY()
		class AJRPGUnit* TargetUnit; // 현재 바라보는 타겟 유닛. ★★

	UPROPERTY()
		bool bButtonVisible; // 버튼이 활성화 되어있는지

	int32 TargetNumber;

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, const float InDeltaTime) override;


	void Init();
	void PlayPriority(); // 배틀이 시작되고 위젯 실행.
	void SetUnitList(); // 게임할때 Hidden 하고 다시 Visible 하면 애초에 바뀐상태로 UnitList가 되어있음.
	void EnermyListBeginInit(); // 맨 처음에 현재 존재하는 적 리스트 초기화.
	void EnermyListInit(); // 적 리스트 초기화.

	void TargetToRotation(); // 캐릭터의 회전을 다시 락온쪽으로 돌림.
	void EnermyTargetToRotation();

	void SetVisible(bool bFlag);
	void EnermyTurnHidden(bool bFlag); // 적 차례시 위젯 단순화

	//void SetButtonVisible(bool bFlag);

	void SetLockOn(int32 Num = 0); // 락온 UI 띄우기 함수.
	void HiddenLockOn();

	// 배틀 턴 시작
	void BattleTurnInit();
	void EnermyTurnFirst();


};
