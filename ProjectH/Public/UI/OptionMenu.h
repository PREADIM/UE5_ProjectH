// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Blueprint/UserWidget.h"
#include "OptionMenu.generated.h"

/**
 * 
 */

/* ★ 각종 옵션 세팅에 관한 UI 해당 셋팅을 MainGameSetting.ini에 저장한다. ★*/
UCLASS()
class PROJECTH_API UOptionMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
		class UProjectHGameInstance* GI;

	/* 바인드 위젯 */
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


	/* 현재 선택한 것을 표시할 문자열 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Resolution;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 AASetting;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 ShadowSetting;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 TextureSetting;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MouseSensitivity; // 0~ 1사이 여야한다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxMouseSensitivity; // 최대값

	/* 선택한 문자열을 콘솔 명령어로 표현할 문자열 */
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


	/* 셋팅이 변경되었는지 플래그 변수 */
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

	void AddViewportSetupComboBox(); /* AddToViewport 함과동시에 SetComboBox실행 */


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
