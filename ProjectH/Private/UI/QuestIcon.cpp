// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/QuestIcon.h"
#include "Components/TextBlock.h"

void UQuestIcon::NativeConstruct()
{
	Super::NativeConstruct();
}

void UQuestIcon::Init(float Dist)
{
	Distance = Dist; // ���� �Ÿ����� �� SetText�� �ϱ⸦ �����ϱ�����.
	FString DS = FString::FromInt(Dist);
	DistanceText->SetText(FText::FromString(DS));
}