// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Blueprint/UserWidget.h"
#include "JRPGESCMenu.generated.h"

/**
 * 
 */
// ★★ MainUI 상태의 ESC UI
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
		TSubclassOf<class UJRPGPartySettingUI> BP_PartySettingUI;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UJRPGPartySettingUI* PartySettingUI; // 파티 셋팅하는 UI


	UPROPERTY(VisibleAnywhere)
		class UJRPGMainWidget* OwnerMainUI;



public:
	virtual void SetCloseFunction() {}

	void Init();

	UFUNCTION()
		void PartySet();
	UFUNCTION()
		void QuitTema();

};
