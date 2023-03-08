// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Tema/JRPG/CustomWidget.h"
#include "Blueprint/UserWidget.h"
#include "JRPGPartySettingUI.generated.h"

/**
 * 
 */
UCLASS(Meta = (DisalbeNativeTick))
class PROJECTH_API UJRPGPartySettingUI : public UCustomWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere)
		class UJRPGMainWidget* OwnerMainUI;


	UPROPERTY(meta = (BindWidget))
		class UButton* Exit;

	UPROPERTY()
		class APartySettingField* PartyField;

public:
	virtual void SetCloseFunction() override;

	void Init();

	UFUNCTION()
		void ExitWidget();	
};
