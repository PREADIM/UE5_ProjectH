// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Blueprint/UserWidget.h"
#include "JRPGTemaUI.generated.h"

/**
 * 
 */
// �ڡ� ���� �ֻ����� ��ġ�� ���� ����. ����ȿ� �ٵ������. ���� UI�� BattleUI��
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
};