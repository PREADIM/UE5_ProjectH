// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Tema/JRPG/CustomWidget.h"
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

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UButton* Out; // 해제하기 버튼
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UButton* Select; // 선택하기 버튼

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* WarningText;

	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* PlayWarning;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UJRPGSettingHoriList> BP_HoriList; // 리스트목록에 뜰 캐릭터 아이콘들을 모은것
	
	UPROPERTY()
		int32 SetCharNum; // 선택된 캐릭터 넘버
	UPROPERTY()
		int32 SelectFieldNumber; // 선택된 캐릭터 넘버

	UPROPERTY()
		class APartySettingField* OwnerField;

	// 이 위젯은 JRPGSettingPartyIcon에서 호버됐을때 캐릭터의 디테일을 보여주는 용도이지만,
	// 마우스가 호버되는 위치에따라 해당 아이콘또한 움직이기 위해서 호버되자마자 위젯을 가져와서
	// 해당 위젯의 위치를 Tick으로 움직인다.
	UPROPERTY()
		class UPartyHoveredWidget* HoveredWidget;
	
	
	/*-------------------
		Widget Sound
	---------------------*/

	UPROPERTY(EditAnywhere)
		class USoundBase* ClickSound;

public:
	virtual void NativeTick(const FGeometry& MyGeometry, const float InDeltaTime) override;
	virtual void SetCloseFunction() override;

	void Init();
	void SetList();

	UFUNCTION()
		void ExitWidget();


	void SetVisibilityOutButton(bool bFlag); // 해제하기 버튼 비지빌리티 설정.
	void SetVisibilitySelectButton(bool bFlag); // 선택하기 버튼 비지빌리티 설정.

	UFUNCTION()
		void SetPartyOut();
	UFUNCTION()
		void SetPartyChange();

	void SetCurrentPartySave(); // 새롭게 파티 설정.

	// ★영웅을 클릭했을때 선택하기 , 해제하기 버튼을 따로 만들자. Hidden으로 버튼을 숨겼다가 누르면 Visible하기.
	// 선택하기 해제하기 버튼은 SetPartyChar()함수를 OnClicked에 가지고있는다.
	// 
	// 해제하기는 무조건 파티리스트가 줄어들기때문에 그 뒤에있는 캐릭터를 앞으로 가지고 와야한다.
	// 선택하기는 파티리스트는 줄어들지않고, 캐릭터 위치가 변하거나, 새롭게 스폰하는 것이므로,
	// 이미 있는 캐릭터인지 판단하고 있으면 서로 자리만 교체하고, 없으면 Destroy후에 CurrentParty에 번호 할당.

	void PlayWarningText(); // 다 찻다고 경고창 띄우기.
};
