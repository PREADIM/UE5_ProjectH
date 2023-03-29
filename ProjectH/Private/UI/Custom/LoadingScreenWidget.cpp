// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Custom/LoadingScreenWidget.h"
#include "Components/Image.h"

void ULoadingScreenWidget::SetScreenImage(FString OpenLevel)
{
	if (OpenLevel == "MainLevel")
	{
		ScreenImage->SetBrushFromTexture(MainLevelScreen);
		return;
	}

	if (OpenLevel == "MainLevel")
	{
		ScreenImage->SetBrushFromTexture(JRPGScreen);
		return;
	}

	if (OpenLevel == "ARPGMap")
	{
		ScreenImage->SetBrushFromTexture(ARPGScreen);
		return;
	}
}