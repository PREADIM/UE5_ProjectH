// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Blueprint/UserWidget.h"
#include "ESCMenu.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UESCMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class APlayerController* OwnerController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class TSubclassOf<class UOptionMenu> BP_OptionMenu;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UOptionMenu* OptionMenu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class TSubclassOf<class UHelpMenu> BP_HelpMenu;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UHelpMenu* HelpMenu;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UCanvasPanel* SettingCanvas;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UButton* Resome;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UButton* Setting;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UButton* Help;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UButton* Quit;


	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* ESCMenuFade;


	bool bOptionKeyOpen = false; // �ɼ�â�� Ű�� �����ٸ� �ڷΰ��ⴭ������ escâ�� �߸�ȵǱ⶧��.


public:
	//virtual void NativeConstruct() override;
	void Init();

	UFUNCTION()
		void ResomeClick();
	UFUNCTION()
		void SettingClick();
	UFUNCTION()
		void HelpClick();
	UFUNCTION()
		void QuitClick();
	UFUNCTION()
		void PrevClick();

	void SettingKeyClick(); // ����Ű�� �ɼ�Ű �����Ҷ�.


	UFUNCTION()
		void SetMouseOff();
	UFUNCTION()
		void HelpUIOff();

	void ESCMenuAnimation(bool IsOpened);

};
