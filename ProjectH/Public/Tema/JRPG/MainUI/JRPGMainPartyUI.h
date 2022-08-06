// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Blueprint/UserWidget.h"
#include "Tema/JRPG/CustomWidget.h"
#include "JRPGMainPartyUI.generated.h"

/**
 * 
 */

// ���� ������ �����ʿ� �� ��Ƽ ����Ʈ ����. ��Ʈ�ѷ��� CurrentParty
// JRPGPartySlot�� ��� ���.
UCLASS()
class PROJECTH_API UJRPGMainPartyUI : public UCustomWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere)
		TArray<int32> CurrentParty;

	UPROPERTY(VisibleAnywhere)
		TArray<class UJRPGPartySlot*> PartySlots; 
	// �ش� ��Ƽ ������ ĭ������ �ִϸ��̼� �� ������ ���� �ε����� ������ ����.


	UPROPERTY(meta = (BindWidget))
		class UVerticalBox* MainPartyList; // ���� UI���� �����ʿ� ���� ������ ��Ƽ����Ʈ�� �߰��ϱ�.

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UJRPGPartySlot> BP_PartySlot;


public:
	virtual void SetCloseFunction() {}

	void Init();
	void SetCurrentPartyList(); // ���� ��Ƽ����Ʈ�� UI�� �������� �۾�.
	
};
