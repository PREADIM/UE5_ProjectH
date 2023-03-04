// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameMode/ProjectHGameMode.h"
#include "ProjectHHUD.h"
#include "Character/ProjectHCharacter.h"
#include "UObject/ConstructorHelpers.h"

AProjectHGameMode::AProjectHGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	//DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AProjectHHUD::StaticClass();
}

void AProjectHGameMode::PostLogin(APlayerController* Login)
{
	Super::PostLogin(Login);

	if (Login != nullptr)
	{
		Login->SetInputMode(FInputModeGameOnly());
		Login->SetShowMouseCursor(false);
	}
}


