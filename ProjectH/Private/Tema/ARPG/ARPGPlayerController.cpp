// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/ARPGPlayerController.h"
#include "Tema/ARPG/ARPGGameMode.h"
#include "Tema/ARPG/ARPGUnit.h"
#include "Blueprint/UserWidget.h"

AARPGPlayerController::AARPGPlayerController()
{

}

void AARPGPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	OwnerUnit = Cast<AARPGUnit>(InPawn);
	if (OwnerUnit == nullptr)
	{
		_DEBUG("Not OwnerUnit");
	}

	GM = Cast<AARPGGameMode>(GetWorld()->GetAuthGameMode());
	if (GM && OwnerUnit)
	{
		OwnerUnit->GM = GM;
	}


}

void AARPGPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (BP_LockOnUI)
	{
		LockOnUI = CreateWidget<UUserWidget>(GetWorld(), BP_LockOnUI);
	}

}


void AARPGPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

}


void AARPGPlayerController::SetLockPosition(FVector2D Pos)
{
	if (LockOnUI)
	{
		LockOnUI->SetPositionInViewport(Pos);
	}
}

void AARPGPlayerController::LockOnAddViewport(bool bFlag)
{
	if (!LockOnUI)
		return;

	if (bFlag)
	{
		LockOnUI->AddToViewport();
	}
	else
	{
		LockOnUI->RemoveFromParent();
	}
}
