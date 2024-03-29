// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Tema/JRPG/CustomWidget.h"
#include "JRPGTemaUI.generated.h"

/**
 * 
 */
// ★★ 가장 최상위에 위치한 메인 위젯. 여기안에 다들어있음. 메인 UI와 BattleUI등
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

	void CreateMainWidget();
	void CreateBattleWidget();

	void StartBattleWidget();
	void StartMainWidget();
	bool MainIsInViewport();
	bool BattleIsInViewport();

	void SetPartyChange(); // 파티 리스트 새롭게 변경.


	//배틀 상황일때
	void PlayBattleWidget();
	void BattleTurnStart(bool bFlag);

	/*---------------------------------------------
		PlayerController->BattlWidget의 인터페이스
	-----------------------------------------------*/

	void SetVisibleBattleWidget(bool bFlag);
	void SkillAndListButtonHidden(bool bFlag);
	void EnermyListSetup();
	void HiddenLockOn();
	void BattleUIOnOff(bool bOnOff);
	void PlayPriority();

	/* 버프아이콘을 해당 캐릭터 PriorityList에서 아이콘을 셋업 해준다.*/
	void BuffSetupPrioirtyList(class AJRPGUnit* Unit);



};
