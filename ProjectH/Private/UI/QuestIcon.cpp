// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/QuestIcon.h"
#include "Components/TextBlock.h"

void UQuestIcon::NativeConstruct()
{
	Super::NativeConstruct();
}

void UQuestIcon::Init(float Dist)
{
	Distance = Dist; // 같은 거리에서 또 SetText를 하기를 방지하기위함.
	FString DS = FString::FromInt(Dist);
	DistanceText->SetText(FText::FromString(DS));
}