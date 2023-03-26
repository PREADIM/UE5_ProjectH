// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Blueprint/UserWidget.h"
#include "Tema/JRPG/CustomWidget.h"
#include "JRPGESCMenu.generated.h"

/**
 * 
 */
// ¡Ú¡Ú MainUI »óÅÂÀÇ ESC UI
UCLASS(Meta = (DisableNativeTick))
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
	UPROPERTY(BlueprintReadWrite)
		class APartySettingField* PartyField;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UUserWidget> BP_QuitWidget;
	UPROPERTY()
		class UUserWidget* QuitWidget;
	UPROPERTY()
		class UJRPGMainWidget* OwnerMainUI;


	UPROPERTY(EditAnywhere)
		class USoundBase* ClickSound;

public:
	virtual void SetCloseFunction() override;
	void Init();

	UFUNCTION()
		void ResomeFunc();
	UFUNCTION()
		void PartySet();
	UFUNCTION()
		void QuitTema();

	void SpawnPartyField();

	
};
