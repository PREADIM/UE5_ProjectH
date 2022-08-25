// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/BattleUI/JRPGPriority.h"
#include "Tema/JRPG/BattleUI/JRPGBattleUnitIcon.h"
#include "Tema/JRPG/JRPGPlayerController.h"
#include "Components/VerticalBox.h"


void UJRPGPriority::PlayInit()
{
	if (BP_UnitIcon && GM)
	{	
		float WaitTime = 0.2f;
		Units = GM->UnitList;
		UnitList->ClearChildren();
		cnt = 0;

		GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			if (Units.Num() > cnt)
			{
				UJRPGBattleUnitIcon* Icon = CreateWidget<UJRPGBattleUnitIcon>(GetWorld(), BP_UnitIcon);
				if (Icon)
				{
					FJRPGUnitUIStruct* UnitUI = OwnerController->GetUnitUI(Units[cnt].Unit->CharNum);
					// 색다른 UI를 쓰려면 여기에 추가해서 사용해도 될듯.

					if (UnitUI)
					{
						Icon->Init(UnitUI->CharTex, UnitUI->CharName);
						Icon->SetPadding(FMargin(0.f, 0.f, 0.f, 5.f));
						UnitList->AddChild(Icon);
						Icons.Add(Icon);
					}
				}
				cnt++;
			}
			else
			{					
				GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
				_DEBUG("Clear");
			}
		}), WaitTime, true);


		FTimerHandle Handle2; // currentAnim을 위한 타이머
		float WaitTime2 = 1.8f;
		GetWorld()->GetTimerManager().SetTimer(Handle2, this, &UJRPGPriority::PlayCurrentUnit, WaitTime2, false);
	}

}



// 평상시 PartyList Set
void UJRPGPriority::SetUnitList()
{
	Units = GM->UnitList;
	UnitList->ClearChildren();

	for (int32 i = 0; i < Units.Num(); i++)
	{

		UJRPGBattleUnitIcon* Icon = CreateWidget<UJRPGBattleUnitIcon>(GetWorld(), BP_UnitIcon);
		if (Icon)
		{
			FJRPGUnitUIStruct* UnitUI = OwnerController->GetUnitUI(Units[i].Unit->CharNum);
			// 색다른 UI를 쓰려면 여기에 추가해서 사용해도 될듯.

			if (UnitUI)
			{
				Icon->Init(UnitUI->CharTex, UnitUI->CharName);
				Icon->SetPadding(FMargin(0.f, 0.f, 0.f, 5.f));
				UnitList->AddChild(Icon);
				Icons.Add(Icon);
			}
		}
	}

	FTimerHandle Handle2; // currentAnim을 위한 타이머
	float WaitTime2 = 0.3f;
	GetWorld()->GetTimerManager().SetTimer(Handle2, this, &UJRPGPriority::PlayCurrentUnit, WaitTime2, false);
}

void UJRPGPriority::PlayCurrentUnit()
{
	if (Icons.IsValidIndex(0))
	{
		FTimerHandle TurnStartHandle;
		float WaitTime = 0.2f;
		Icons[0]->PlayCurrentAnim();
		GetWorld()->GetTimerManager().SetTimer(TurnStartHandle, GM, &AJRPGGameMode::TurnStart, WaitTime, false);
	}
}

