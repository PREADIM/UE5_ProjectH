// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Blueprint/UserWidget.h"
#include "JRPGSettingPartyIcon.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UJRPGSettingPartyIcon : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UImage* CharIcon;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UButton* Button;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UTexture2D* NormalTex; // ��ĭ �ؽ���

	UPROPERTY()
		int32 CharNum;

	UPROPERTY()
		class APartySettingField* OwnerField;
	UPROPERTY()
		class AJRPGPlayerController* OwnerController;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FLinearColor SelectColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FLinearColor NormalColor;

	UPROPERTY()
		class UJRPGSettingPartySlot* OwnerWidget; // ������ ȣ�������� ���� �뵵.

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UPartyHoveredWidget> BP_HoveredWidget;
	UPROPERTY()
		class UPartyHoveredWidget* HoveredWidget;

public:
	void Init(UTexture2D* Texture);

	UFUNCTION()
		void SetPartyList();
	UFUNCTION()
		void SetHoveredWidget();
	UFUNCTION()
		void SetUnHoveredWidget();

	void SetSelectColor(bool bFlag); // ���õ� �������� �÷��� ������. 
};
