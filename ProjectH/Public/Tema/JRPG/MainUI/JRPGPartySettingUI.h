// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Blueprint/UserWidget.h"
#include "JRPGPartySettingUI.generated.h"

/**
 * 
 */
// �ڡ� ��Ƽ ������ �ϴ� UI
UCLASS()
class PROJECTH_API UJRPGPartySettingUI : public UCustomWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere)
		class UJRPGMainWidget* OwnerMainUI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class APartySettingField> BP_PartyField;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class APartySettingField* PartyField;

	UPROPERTY()
		TArray<int32> CurrentParty;
	UPROPERTY(meta = (BindWidget))
		class UButton* Exit;


	/* ĳ���� ��Ƽ����â���� �����ֱ������ ���� ĳ���� ����Ʈ */
	UPROPERTY()
		TArray<class AJRPGUnit*> SpawnChars;

public:
	virtual void SetCloseFunction() override;

	void Init();
	void SetCurrentParty(); // ���� ��Ƽ ����� ����

	UFUNCTION()
		void ExitWidget();	

};
