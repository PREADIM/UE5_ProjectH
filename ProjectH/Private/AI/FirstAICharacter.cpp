// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/FirstAICharacter.h"
#include "AI/ProjectHAIController.h"



AFirstAICharacter::AFirstAICharacter()
{

}



void AFirstAICharacter::BeginPlay()
{
	Super::BeginPlay();
	

}

// Called every frame
void AFirstAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFirstAICharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}