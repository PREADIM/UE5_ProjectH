// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/MainUI/JRPGSettingPartyIcon.h"
#include "Components/Image.h"
#include "Tema/JRPG/MainUI/PartySettingField.h"


void UJRPGSettingPartyIcon::Init(class UTexture2D* Texture)
{
	if (Texture && OwnerField)
	{
		CharIcon->SetBrushFromTexture(Texture);

		// ��ư Ŭ���� PartyField�� SetSpawnUnit�� �����ϰ�, �ش� ĭ�� ������ �ٽ� �Ѵ�.
		// �߿��Ѱ��� ���� 1 2 3�� ĭ�� 1��ĭ�� ��� 2�� 3���� 1 �� 2���� �Ǵ� ������ �ؾ��Ѵ�.
		// �Ǵ� ������ ĳ���Ͱ� �ش� ��Ƽ�� ������� ���� �ڸ��� �ٲ���Ѵ�.


	}
}


void UJRPGSettingPartyIcon::SetPartyList()
{
	OwnerField->SetPartyList(CharNum);
}