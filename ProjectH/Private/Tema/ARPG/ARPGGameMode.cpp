// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/ARPGGameMode.h"
#include "Kismet/GameplayStatics.h"

AARPGGameMode::AARPGGameMode()
{

}

void AARPGGameMode::PostLogin(APlayerController* Login)
{
	Super::PostLogin(Login);

	if (Login)
	{
		Login->SetInputMode(FInputModeGameOnly());
		Login->SetShowMouseCursor(false);
	}

}


void AARPGGameMode::Restart()
{
	FString Name = UGameplayStatics::GetCurrentLevelName(GetWorld());
	UGameplayStatics::OpenLevel(GetWorld(), FName(*Name));
}