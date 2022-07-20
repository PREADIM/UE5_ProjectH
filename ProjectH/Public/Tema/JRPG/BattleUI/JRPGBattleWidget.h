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

public:
	virtual void SetCloseFunction() {}
	void Init();
	//float GetESCRenderOpacity();
	
};
