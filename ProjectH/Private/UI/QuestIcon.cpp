// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/QuestIcon.h"
#include "Components/TextBlock.h"

void UQuestIcon::NativeConstruct()
{
	Super::NativeConstruct();
}

void UQuestIcon::Init()
{
	FString DS = FString::FromInt(Distance);
	DistanceText->SetText(FText::FromString(DS));
}