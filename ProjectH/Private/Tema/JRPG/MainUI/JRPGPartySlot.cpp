// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/MainUI/JRPGPartySlot.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UJRPGPartySlot::Init(UTexture2D* tex, FString Name, int32 Num)
{
	UnitProfile->SetBrushFromTexture(tex);
	UnitName->SetText(FText::FromString(Name));
	KeyText->SetText(FText::FromString(FString::FromInt(Num)));
}