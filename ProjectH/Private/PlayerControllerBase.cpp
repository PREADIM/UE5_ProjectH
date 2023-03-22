// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllerBase.h"



void APlayerControllerBase::MouseOn()
{
	int32 x, y;
	GetViewportSize(x, y);
	SetMouseLocation(UKismetMathLibrary::FTrunc(x / 2), UKismetMathLibrary::FTrunc(y / 2));
	SetShowMouseCursor(true);
	SetInputMode(FInputModeGameAndUI());
}


void APlayerControllerBase::MouseOff()
{
	SetShowMouseCursor(false);
	SetInputMode(FInputModeGameOnly());
}