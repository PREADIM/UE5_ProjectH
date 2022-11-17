// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Tema/JRPG/CustomWidget.h"
#include "JRPGTemaUI.generated.h"

/**
 * 
 */
// ★★ 가장 최상위에 위치한 메인 위젯. 여기안에 다들어있음. 메인 UI와 BattleUI등
UCLASS()
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

	void SetPartyChange(); // 파티 리스트 새롭게 변경.

	void EnermyListSetup();

	//배틀 상황일때
	void PlayBattleWidget();
	void BattleTurnStart(bool bFlag);
	void SetVisibleBattleWidget(bool bFlag); // 배틀위젯 자체를 Hidden
	void EnermyTurnWidget(bool bFlag); // 적 턴일때 보이게할 위젯만 키기

	void TargetToRotation(); // 적 쪽으로 몸 돌리기.
	void EnermyTargetToRotation();

	void HiddenRockOn(); // 전체 스킬을 사용할때 락온을 가리는 용도.
};
