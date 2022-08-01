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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UTexture2D* NormalTex; // ��ĭ �ؽ���




public:
	void Init(UTexture2D* Texture);
};