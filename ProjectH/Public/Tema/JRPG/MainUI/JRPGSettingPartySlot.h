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
		class UButton* Out; // �����ϱ� ��ư
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UButton* Select; // �����ϱ� ��ư

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* WarningText;

	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* PlayWarning;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UJRPGSettingHoriList> BP_HoriList; // ����Ʈ��Ͽ� �� ĳ���� �����ܵ��� ������
	
	UPROPERTY()
		int32 SetCharNum; // ���õ� ĳ���� �ѹ�
	UPROPERTY()
		int32 SelectFieldNumber; // ���õ� ĳ���� �ѹ�

	UPROPERTY()
		class APartySettingField* OwnerField;

	// �� ������ JRPGSettingPartyIcon���� ȣ�������� ĳ������ �������� �����ִ� �뵵������,
	// ���콺�� ȣ���Ǵ� ��ġ������ �ش� �����ܶ��� �����̱� ���ؼ� ȣ�����ڸ��� ������ �����ͼ�
	// �ش� ������ ��ġ�� Tick���� �����δ�.
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


	void SetVisibilityOutButton(bool bFlag); // �����ϱ� ��ư ��������Ƽ ����.
	void SetVisibilitySelectButton(bool bFlag); // �����ϱ� ��ư ��������Ƽ ����.

	UFUNCTION()
		void SetPartyOut();
	UFUNCTION()
		void SetPartyChange();

	void SetCurrentPartySave(); // ���Ӱ� ��Ƽ ����.

	// �ڿ����� Ŭ�������� �����ϱ� , �����ϱ� ��ư�� ���� ������. Hidden���� ��ư�� ����ٰ� ������ Visible�ϱ�.
	// �����ϱ� �����ϱ� ��ư�� SetPartyChar()�Լ��� OnClicked�� �������ִ´�.
	// 
	// �����ϱ�� ������ ��Ƽ����Ʈ�� �پ��⶧���� �� �ڿ��ִ� ĳ���͸� ������ ������ �;��Ѵ�.
	// �����ϱ�� ��Ƽ����Ʈ�� �پ�����ʰ�, ĳ���� ��ġ�� ���ϰų�, ���Ӱ� �����ϴ� ���̹Ƿ�,
	// �̹� �ִ� ĳ�������� �Ǵ��ϰ� ������ ���� �ڸ��� ��ü�ϰ�, ������ Destroy�Ŀ� CurrentParty�� ��ȣ �Ҵ�.

	void PlayWarningText(); // �� ���ٰ� ���â ����.
};
