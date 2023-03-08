// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Tema/JRPG/CustomWidget.h"
#include "EnermyIconButton.generated.h"

/**
 * 
 */
UCLASS(Meta = (DisalbeNativeTick))
class PROJECTH_API UEnermyIconButton : public UCustomWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
		class UButton* EnermyButton;

	UPROPERTY(EditAnywhere)
		FLinearColor NormalColor;
	UPROPERTY(EditAnywhere)
		FLinearColor HoveredColor;
	UPROPERTY(EditAnywhere)
		FLinearColor PressedColor;

	UPROPERTY()
		int32 Num;

	UPROPERTY()
		class UJRPGBattleWidget* OwnerWidget; // 관찰자 
	// 다시 카메라 제자리로 돌아올때 사용
public:
	void Init(class UTexture2D* CharTex, int32 cnt);

	UFUNCTION()
		void TargetLockOn();
	
};
