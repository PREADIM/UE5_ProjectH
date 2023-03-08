// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/BattleUI/JRPGBuffWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Tema/JRPG/JRPGGameMode.h"

void UJRPGBuffWidget::Init(class UDebuffClass* DebuffClass, EIconSizeType SizeType)
{
	AJRPGGameMode* GM = Cast<AJRPGGameMode>(GetWorld()->GetAuthGameMode());

	if (!GM)
		return;

	FString IconName;
	FSlateBrush Brush;
	switch (DebuffClass->DebuffState)
	{
	case EDebuffState::ATK_DOWN:
		IconName = FString("ATK_DOWN");
		break;
	case EDebuffState::DEF_DOWN:
		IconName = FString("DEF_DOWN");
		break;
	case EDebuffState::STUN:
		IconName = FString("STUN");
		break;
	}

	FBuffIconStruct* IconStruct = GM->GetBuffIcon(IconName);
	if (IconStruct)
	{
		Brush.SetResourceObject(IconStruct->IconTexture);
		switch (SizeType)
		{
		case EIconSizeType::Mini:
			Brush.SetImageSize(FVector2D(20.f, 20.f));
			break;
		case EIconSizeType::Midium:
			Brush.SetImageSize(FVector2D(32.f, 32.f));
			break;
		case EIconSizeType::Big:
			Brush.SetImageSize(FVector2D(64.f, 64.f));
		}
	}

	BuffImg->SetBrush(Brush);
	CntText->SetText(FText::FromString(FString::FromInt(DebuffClass->CurrentDebuffTurn)));
}