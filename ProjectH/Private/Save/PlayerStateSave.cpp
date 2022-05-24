// Fill out your copyright notice in the Description page of Project Settings.


#include "Save/PlayerStateSave.h"
#include "Kismet/GameplayStatics.h"


const FString UPlayerStateSave::SlotName = FString("PlayerSave");


UPlayerStateSave::UPlayerStateSave()
{

}

void UPlayerStateSave::LoadPlayerQuest(TArray<int32>* QuestNums, TArray<int32>* FinishedQuests)
{
	*QuestNums = QuestNumbers;
	*FinishedQuests = FinishedQuestNumbers;
}

void UPlayerStateSave::SavePlayerQuest(TArray<int32>* QuestNums, TArray<int32>* FinishedQuests)
{
	QuestNumbers = *QuestNums;
	FinishedQuestNumbers = *FinishedQuests;

	SaveSlot();
}


void UPlayerStateSave::SaveSlot()
{
	UGameplayStatics::SaveGameToSlot(this, SlotName, 0);
}