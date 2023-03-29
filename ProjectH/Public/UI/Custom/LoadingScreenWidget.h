// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Blueprint/UserWidget.h"
#include "LoadingScreenWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API ULoadingScreenWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
		class UImage* ScreenImage;

	UPROPERTY(EditAnywhere)
		class UTexture2D* MainLevelScreen;

	UPROPERTY(EditAnywhere)
		class UTexture2D* JRPGScreen;

	UPROPERTY(EditAnywhere)
		class UTexture2D* ARPGScreen;
	

public:
	void SetScreenImage(FString OpenLevel);
};
