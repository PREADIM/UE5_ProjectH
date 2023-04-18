// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Tema/JRPG/CustomWidget.h"
#include "JRPGMainWidget.generated.h"

/**
 * 
 */
//배틀 상태가 아닌 평상시의 위젯의 총책임자.
UCLASS(Meta = (DisableNativeTick))
class PROJECTH_API UJRPGMainWidget : public UCustomWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
		class UJRPGESCMenu* ESCMenu;
	UPROPERTY(meta = (BindWidget))
		class UVerticalBox* NoticeBoard;
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* ESCAnim;
	UPROPERTY(meta = (BindWidget))
		class UJRPGMainPartyUI* MainPartyUI;

public:
	virtual void SetCloseFunction() {}

	void Init();
	void PlayESCAnim(bool bPlay);

	UFUNCTION()
		void PlayESC(); // ESC 실행
	UFUNCTION()
		void ReverseESC(); // ESC끄기

	float GetESCRenderOpacity();

	void SetPartyChange(); // 파티셋팅에서 파티가 변경되었으니 MainUI에서도 변경.

	class UVerticalBox* GetNoticeBoard() { return NoticeBoard; }
	
};
