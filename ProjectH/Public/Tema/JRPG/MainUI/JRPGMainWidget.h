// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Blueprint/UserWidget.h"
#include "JRPGMainWidget.generated.h"

/**
 * 
 */
// �ڡ� ��Ʋ ���°� �ƴ� ������ ������ ��å����.
UCLASS()
class PROJECTH_API UJRPGMainWidget : public UCustomWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
		class UJRPGESCMenu* ESCMenu;

	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* ESCAnim;

	UPROPERTY(meta = (BindWidget))
		class UJRPGMainPartyUI* MainPartyUI;



public:
	virtual void SetCloseFunction() {}

	void Init();

	void PlayESCAnim(bool bPlay);

	UFUNCTION()
		void PlayESC(); // ESC ����
	UFUNCTION()
		void ReverseESC(); // ESC����

	float GetESCRenderOpacity();
	void SetMouseOff();
	void MouseOff();
	
	
};