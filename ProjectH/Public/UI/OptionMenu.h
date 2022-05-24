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

	/* ������ ���ڿ��� �ܼ� ��ɾ�� ǥ���� ���ڿ� */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString SelectResolution;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 SelectAASetting;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 SelectShadowSetting;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 SelectTextureSetting;

	UPROPERTY(EditAnywhere, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* OptionFade;


	/* ������ ����Ǿ����� �÷��� ���� */
	bool bRes;
	bool bAA;
	bool bShadow;
	bool bTex;

public:
	void Init();
	void AddComboBoxList();
	void SetComboBox();
	void SetINI();


	UFUNCTION(BlueprintCallable)
		void SetResCommand(FString str);
	UFUNCTION(BlueprintCallable)
		void SetAACommand(int32 str);
	UFUNCTION(BlueprintCallable)
		void SetShadowCommand(int32 str);
	UFUNCTION(BlueprintCallable)
		void SetTextureCommand(int32 str);

	UFUNCTION()
		void Apply();


	void OptionAnimation(bool IsOpened);
	void OptionMenuRemove();

};
