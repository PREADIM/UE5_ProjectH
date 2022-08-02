// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Blueprint/UserWidget.h"
#include "JRPGSettingPartySlot.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UJRPGSettingPartySlot : public UCustomWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UScrollBox* Scroll;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UButton* Exit;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UJRPGSettingHoriList> BP_HoriList; // 리스트목록에 뜰 캐릭터 아이콘들을 모은것
	UPROPERTY()
		int32 HaveCharNum;

	UPROPERTY()
		class APartySettingField* OwnerField;

public:
	void Init();
	void SetList();

	UFUNCTION()
		void ExitWidget();

	virtual void SetCloseFunction();

	// ★영웅을 클릭했을때 선택하기 , 해제하기 버튼을 따로 만들자. Hidden으로 버튼을 숨겼다가 누르면 Visible하기.
	// 선택하기 해제하기 버튼은 SetPartyChar()함수를 OnClicked에 가지고있는다.


	// 해제하기는 무조건 파티리스트가 줄어들기때문에 그 뒤에있는 캐릭터를 앞으로 가지고 와야한다.
	// 선택하기는 파티리스트는 줄어들지않고, 캐릭터 위치가 변하거나, 새롭게 스폰하는 것이므로,
	// 이미 있는 캐릭터인지 판단하고 있으면 서로 자리만 교체하고, 없으면 Destroy후에 CurrentParty에 번호 할당.
};
