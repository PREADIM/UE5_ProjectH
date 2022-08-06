// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Blueprint/UserWidget.h"
#include "Tema/JRPG/CustomWidget.h"
#include "JRPGMainPartyUI.generated.h"

/**
 * 
 */

// 메인 위젯의 오른쪽에 뜰 파티 리스트 슬롯. 컨트롤러의 CurrentParty
// JRPGPartySlot을 묶어서 사용.
UCLASS()
class PROJECTH_API UJRPGMainPartyUI : public UCustomWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere)
		TArray<int32> CurrentParty;

	UPROPERTY(VisibleAnywhere)
		TArray<class UJRPGPartySlot*> PartySlots; 
	// 해당 파티 슬롯의 칸마다의 애니메이션 및 설정을 위해 인덱스로 가지고 있음.


	UPROPERTY(meta = (BindWidget))
		class UVerticalBox* MainPartyList; // 메인 UI에서 오른쪽에 현재 선택한 파티리스트가 뜨게하기.

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UJRPGPartySlot> BP_PartySlot;


public:
	virtual void SetCloseFunction() {}

	void Init();
	void SetCurrentPartyList(); // 현재 파티리스트를 UI로 띄우기위한 작업.
	
};
