// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/QuestNPCAI.h"
#include "Character/ProjectHCharacter.h"
#include "Controller/ProjectH_PC.h"
#include "UI/MainQuestUI.h"

void AQuestNPCAI::BeginPlay()
{
	Super::BeginPlay();

}

void AQuestNPCAI::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

}

void AQuestNPCAI::Interact_Implementation(class AProjectHCharacter* OwnerCharacter)
{
	Super::Interact_Implementation(OwnerCharacter);

	AProjectH_PC* OwnerController = Cast<AProjectH_PC>(OwnerCharacter->GetController());
	if (OwnerController)
	{
		if (!OwnerController->MainQuestUI->bDialogueOpen())
		{
			OwnerController->MainQuestUI->SetDialogueNPC(this);
			OwnerController->MainQuestUI->OpenDialogue();
		}
	}
}