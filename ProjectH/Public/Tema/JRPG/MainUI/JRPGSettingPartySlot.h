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
		TSubclassOf<class UJRPGSettingHoriList> BP_HoriList; // ����Ʈ��Ͽ� �� ĳ���� �����ܵ��� ������
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

	// �ڿ����� Ŭ�������� �����ϱ� , �����ϱ� ��ư�� ���� ������. Hidden���� ��ư�� ����ٰ� ������ Visible�ϱ�.
	// �����ϱ� �����ϱ� ��ư�� SetPartyChar()�Լ��� OnClicked�� �������ִ´�.


	// �����ϱ�� ������ ��Ƽ����Ʈ�� �پ��⶧���� �� �ڿ��ִ� ĳ���͸� ������ ������ �;��Ѵ�.
	// �����ϱ�� ��Ƽ����Ʈ�� �پ�����ʰ�, ĳ���� ��ġ�� ���ϰų�, ���Ӱ� �����ϴ� ���̹Ƿ�,
	// �̹� �ִ� ĳ�������� �Ǵ��ϰ� ������ ���� �ڸ��� ��ü�ϰ�, ������ Destroy�Ŀ� CurrentParty�� ��ȣ �Ҵ�.
};
