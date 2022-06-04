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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Resolution;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 AASetting;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 ShadowSetting;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 TextureSetting;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MouseSensitivity; // 0~ 1���� �����Ѵ�.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxMouseSensitivity; // �ִ밪

	/* ������ ���ڿ��� �ܼ� ��ɾ�� ǥ���� ���ڿ� */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString SelectResolution;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 SelectAASetting;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 SelectShadowSetting;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 SelectTextureSetting;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SelectMouseSensitivity;

	UPROPERTY(EditAnywhere, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* OptionFade;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		APlayerController* OwnerController;


	/* ������ ����Ǿ����� �÷��� ���� */
	bool bRes;
	bool bAA;
	bool bShadow;
	bool bTex;
	bool bMS;

public:
	void Init();
	void AddComboBoxList();
	void SetComboBox();
	void SetOtherOption();
	void SetINI();


	UFUNCTION(BlueprintCallable)
		void SetResCommand(FString str);
	UFUNCTION(BlueprintCallable)
		void SetAACommand(int32 str);
	UFUNCTION(BlueprintCallable)
		void SetShadowCommand(int32 str);
	UFUNCTION(BlueprintCallable)
		void SetTextureCommand(int32 str);
	UFUNCTION(BlueprintCallable)
		void SetMouseSensitivity(float str);

	UFUNCTION()
		void Apply();


	void OptionAnimation(bool IsOpened);
	void OptionMenuRemove();

};
