// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Blueprint/UserWidget.h"
#include "JRPGPartySettingUI.generated.h"

/**
 * 
 */
// ★★ 파티 셋팅을 하는 UI
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


	/* 캐릭터 파티셋팅창에서 보여주기용으로 위한 캐릭터 리스트 */
	UPROPERTY()
		TArray<class AJRPGUnit*> SpawnChars;

public:
	virtual void SetCloseFunction() override;

	void Init();
	void SetCurrentParty(); // 현재 파티 목록을 세팅

	UFUNCTION()
		void ExitWidget();	

};
