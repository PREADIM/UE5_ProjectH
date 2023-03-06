// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Blueprint/UserWidget.h"
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
		class AJRPGUnit* TargetUnit; // ���� �ٶ󺸴� Ÿ�� ����. �ڡ�

	UPROPERTY()
		bool bButtonVisible; // ��ư�� Ȱ��ȭ �Ǿ��ִ���

	int32 TargetNumber;

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, const float InDeltaTime) override;


	void Init();
	void PlayPriority(); // ��Ʋ�� ���۵ǰ� ���� ����.
	void SetUnitList(); // �����Ҷ� Hidden �ϰ� �ٽ� Visible �ϸ� ���ʿ� �ٲ���·� UnitList�� �Ǿ�����.
	void EnermyListBeginInit(); // �� ó���� ���� �����ϴ� �� ����Ʈ �ʱ�ȭ.
	void EnermyListInit(); // �� ����Ʈ �ʱ�ȭ.

	void TargetToRotation(); // ĳ������ ȸ���� �ٽ� ���������� ����.
	void EnermyTargetToRotation();

	void SetVisible(bool bFlag);
	void EnermyTurnHidden(bool bFlag); // �� ���ʽ� ���� �ܼ�ȭ

	//void SetButtonVisible(bool bFlag);

	void SetLockOn(int32 Num = 0); // ���� UI ���� �Լ�.
	void HiddenLockOn();

	// ��Ʋ �� ����
	void BattleTurnInit();
	void EnermyTurnFirst();


};
