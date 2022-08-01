// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/MainUI/JRPGSettingPartyIcon.h"
#include "Components/Image.h"


void UJRPGSettingPartyIcon::Init(class UTexture2D* Texture)
{
	if (Texture)
	{
		CharIcon->SetBrushFromTexture(Texture);
	}
}