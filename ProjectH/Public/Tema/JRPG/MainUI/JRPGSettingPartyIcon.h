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
		class UTexture2D* NormalTex; // 빈칸 텍스쳐

	UPROPERTY()
		int32 CharNum;

	UPROPERTY()
		class APartySettingField* OwnerField;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FLinearColor SelectColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FLinearColor NormalColor;

public:
	void Init(UTexture2D* Texture);

	UFUNCTION()
		void SetPartyList();

	void SetSelectColor(bool bFlag); // 선택된 아이콘은 컬러를 입힌다. 
};
