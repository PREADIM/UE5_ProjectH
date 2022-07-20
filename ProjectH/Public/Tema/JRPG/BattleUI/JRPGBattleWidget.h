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

public:
	virtual void SetCloseFunction() {}
	void Init();
	//float GetESCRenderOpacity();
	
};
