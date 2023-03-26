// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Tema/ARPG/Widget/ARPGWidgetBase.h"
#include "ARPGESCMenu.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UARPGESCMenu : public UARPGWidgetBase
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
		class UButton* Resome;
	UPROPERTY(meta = (BindWidget))
		class UButton* Option;
	UPROPERTY(meta = (BindWidget))
		class UButton* Quit;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* ESCAnim;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UOptionMenu> BP_OptionMenu;
	UPROPERTY()
		class UOptionMenu* OptionMenu;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget> BP_QuitWidget;
	UPROPERTY()
		class UUserWidget* QuitWidget;

	UPROPERTY()
		class APlayerControllerBase* OwnerController;

	bool bESCMenuOpen = false;

	/*----------------------
		Widget Sound
	------------------------*/

	UPROPERTY(EditAnywhere)
		class USoundBase* ClickSound;

public:
	virtual void Init(class AARPGUnitBase* Unit) override;

	void ESCClick();
	void PlayESCAnim(bool bFlag);

	UFUNCTION()
		void ResomeFunc();
	UFUNCTION()
		void OptionFunc();
	UFUNCTION()
		void QuitTema();
	UFUNCTION()
		void ESCMunuRemove();

	UFUNCTION()
		void OptionPrevButton();
};
