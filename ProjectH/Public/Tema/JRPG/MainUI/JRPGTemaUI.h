// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Tema/JRPG/CustomWidget.h"
#include "JRPGTemaUI.generated.h"

/**
 * 
 */
// �ڡ� ���� �ֻ����� ��ġ�� ���� ����. ����ȿ� �ٵ������. ���� UI�� BattleUI��
UCLASS(Meta = (DisableNativeTick))
class PROJECTH_API UJRPGTemaUI : public UCustomWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UJRPGMainWidget> BP_MainWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UJRPGMainWidget* MainWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UJRPGBattleWidget> BP_BattleWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UJRPGBattleWidget* BattleWidget;

public:
	virtual void SetCloseFunction() {}

	void Init();
	void OpenESCMenu();
	void CloseESCMenu();
	void OpenBattleESCMenu();

	void CreateMainWidget();
	void CreateBattleWidget();

	void StartBattleWidget();
	void StartMainWidget();
	bool MainIsInViewport();
	bool BattleIsInViewport();

	void SetPartyChange(); // ��Ƽ ����Ʈ ���Ӱ� ����.


	//��Ʋ ��Ȳ�϶�
	void PlayBattleWidget();
	void BattleTurnStart(bool bFlag);


	//-------------------------------
	/*---------------------------------------------
		PlayerController->BattlWidget�� �������̽�
	-----------------------------------------------*/

	void SetVisibleBattleWidget(bool bFlag);
	void SetEnermyTurnWidget(bool bFlag);
	void EnermyListSetup();
	void HiddenLockOn();
	void EnermySetupLockOnTargetUnit(AJRPGUnit* Target);
	void TargetToRotation();
	void EnermyTargetToRotation();
	void BattleUIOnOff(bool bOnOff);
	void PlayPriority();

};
