// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Blueprint/UserWidget.h"
#include "Tema/JRPG/DebuffClass.h"
#include "JRPGBuffWidget.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EIconSizeType : uint8
{
	Mini UMETA(DisplayName = "20x20"),
	Midium UMETA(DisplayName = "32x32"),
	Big UMETA(DisplayName = "64x64")
};

UCLASS()
class PROJECTH_API UJRPGBuffWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
		class UImage* BuffImg;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* CntText;

public:
	void Init(class UDebuffClass* DebuffClass, EIconSizeType SizeType);
	
};
