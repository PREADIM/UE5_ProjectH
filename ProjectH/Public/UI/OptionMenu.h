// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Blueprint/UserWidget.h"
#include "OptionMenu.generated.h"

/**
 * 
 */

/* �� ���� �ɼ� ���ÿ� ���� UI �ش� ������ MainGameSetting.ini�� �����Ѵ�. ��*/
UCLASS()
class PROJECTH_API UOptionMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
		class UProjectHGameInstance* GI;

	/* ���ε� ���� */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UComboBoxString* ResComboBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UComboBoxString* AAComboBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UComboBoxString* ShadowComboBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UComboBoxString* TextureComboBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UButton* PrevButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UButton* ApplyButton;


	/* ���� ������ ���� ǥ���� ���ڿ� */
	UPROPERTY(BlueprintReadWrite)
		FString Resolution;
	UPROPERTY(BlueprintReadWrite)
		int32 AASetting;
	UPROPERTY(BlueprintReadWrite)
		int32 ShadowSetting;
	UPROPERTY(BlueprintReadWrite)
		int32 TextureSetting;
	UPROPERTY(BlueprintReadWrite)
		float MouseSensitivity; // 0~ 1���� �����Ѵ�.
	UPROPERTY(BlueprintReadWrite)
		float MaxMouseSensitivity; // �ִ밪
	UPROPERTY(BlueprintReadWrite)
		float MasterSoundRaito; // 0~ 1���� �����Ѵ�.


	/* ������ ���ڿ��� �ܼ� ��ɾ�� ǥ���� ���ڿ� */
	UPROPERTY(BlueprintReadWrite)
		FString SelectResolution;
	UPROPERTY(BlueprintReadWrite)
		int32 SelectAASetting;
	UPROPERTY(BlueprintReadWrite)
		int32 SelectShadowSetting;
	UPROPERTY(BlueprintReadWrite)
		int32 SelectTextureSetting;
	UPROPERTY(BlueprintReadWrite)
		float SelectMouseSensitivity;
	UPROPERTY(BlueprintReadWrite)
		float SelectMasterSoundRatio;
	UPROPERTY(EditAnywhere, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* OptionFade;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		APlayerController* OwnerController;

	/*----------------------------------------------*/


	/* ������ ����Ǿ����� �÷��� ���� */
	bool bRes;
	bool bAA;
	bool bShadow;
	bool bTex;
	bool bMS;
	bool bMasterSound;

public:
	void Init();
	void AddComboBoxList();
	void SetComboBox();
	void SetOtherOption();
	void SetINI();

	void AddViewportSetupComboBox(); /* AddToViewport �԰����ÿ� SetComboBox���� */


	UFUNCTION(BlueprintCallable)
		void SetResCommand(FString str);
	UFUNCTION(BlueprintCallable)
		void SetAACommand(int32 str);
	UFUNCTION(BlueprintCallable)
		void SetShadowCommand(int32 str);
	UFUNCTION(BlueprintCallable)
		void SetTextureCommand(int32 str);
	UFUNCTION(BlueprintCallable)
		void SetMouseSensitivity(float ratio);
	UFUNCTION(BlueprintCallable)
		void SetMasterSound(float ratio);
	UFUNCTION(BlueprintImplementableEvent)
		void BindMasterSoundMix(float ratio);

	UFUNCTION()
		void Apply();


	void OptionAnimation(bool IsOpened);
	void OptionMenuRemove();

};
