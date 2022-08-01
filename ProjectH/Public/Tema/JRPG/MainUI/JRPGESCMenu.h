// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Blueprint/UserWidget.h"
#include "JRPGESCMenu.generated.h"

/**
 * 
 */
// �ڡ� MainUI ������ ESC UI
UCLASS()
class PROJECTH_API UJRPGESCMenu : public UCustomWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UButton* Resome;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UButton* PartySetting;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UButton* Quit;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class APartySettingField> BP_PartyField;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class APartySettingField* PartyField;


	UPROPERTY(VisibleAnywhere)
		class UJRPGMainWidget* OwnerMainUI;



public:
	virtual void SetCloseFunction() {}

	void Init();

	UFUNCTION()
		void PartySet();
	UFUNCTION()
		void QuitTema();

	void SpawnPartyField();

	
};