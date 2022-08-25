// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Tema/JRPG/CustomWidget.h"
#include "Tema/JRPG/JRPGGameMode.h"
#include "JRPGPriority.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UJRPGPriority : public UCustomWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
		class UVerticalBox* UnitList;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UJRPGBattleUnitIcon> BP_UnitIcon;

	UPROPERTY()
		TArray<FPriorityUnit> Units;

	UPROPERTY()
		TArray<class UJRPGBattleUnitIcon*> Icons;

	int32 cnt;

	UPROPERTY()
		FTimerHandle TimerHandle; // UI를 반복 호출하는 타이머.

public:
	void PlayInit();
	void SetUnitList();
	void PlayCurrentUnit();

	
};
