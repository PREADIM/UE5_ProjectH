// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllerBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameMode/ProjectHGameInstance.h"



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



void APlayerControllerBase::DisableInputActor(bool bFlag)
{
	if (bFlag)
		GetPawn()->DisableInput(this);
	else
		GetPawn()->EnableInput(this);
}


void APlayerControllerBase::SetNewMouseSensitivity()
{
	if (MouseSensitivity == 0.f)
	{
		class UProjectHGameInstance* GI = Cast<UProjectHGameInstance>(UGameplayStatics::GetGameInstance(this));
		if (GI)
			MouseSensitivity = GI->MS;
	}

	if (MouseSensitivity == 0.f)
		MouseSensitivity = 1.f;
}