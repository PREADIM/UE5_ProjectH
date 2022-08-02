// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/MainUI/JRPGSettingPartyIcon.h"
#include "Components/Image.h"
#include "Tema/JRPG/MainUI/PartySettingField.h"


void UJRPGSettingPartyIcon::Init(class UTexture2D* Texture)
{
	if (Texture && OwnerField)
	{
		CharIcon->SetBrushFromTexture(Texture);

		// 버튼 클릭에 PartyField의 SetSpawnUnit을 연동하고, 해당 칸의 스폰을 다시 한다.
		// 중요한것은 만약 1 2 3번 칸의 1번칸이 비면 2번 3번이 1 번 2번이 되는 식으로 해야한다.
		// 또는 선택한 캐릭터가 해당 파티에 있을경우 서로 자리를 바꿔야한다.


	}
}


void UJRPGSettingPartyIcon::SetPartyList()
{
	OwnerField->SetPartyList(CharNum);
}