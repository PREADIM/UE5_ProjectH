// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/BattleUI/JRPGPriority.h"
#include "Tema/JRPG/BattleUI/JRPGBattleUnitIcon.h"
#include "Tema/JRPG/JRPGPlayerController.h"
#include "Components/VerticalBox.h"
#include "Tema/JRPG/JRPGUnit.h"


void UJRPGPriority::PlayInit()
{
	if (BP_UnitIcon && GM)
	{
		float WaitTime = 0.2f;
		Units = GM->SetUnitList;
		UnitList->ClearChildren();
		Icons.Empty();
		UnitIconIndexs.Empty();
		cnt = 0;

		GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]()
			{
				if (Units.Num() > cnt)
				{
					FPriorityUnit Unit = Units[cnt];
					UJRPGBattleUnitIcon* Icon = CreateWidget<UJRPGBattleUnitIcon>(GetWorld(), BP_UnitIcon);
					if (Icon)
					{
						FJRPGUnitUIStruct* UnitUI = OwnerController->GetUnitUI(Unit.Unit->CharNum);
						// 색다른 UI를 쓰려면 여기에 추가해서 사용해도 될듯.

						if (UnitUI)
						{
							Icon->Unit = Unit.Unit;
							Icon->SetDebuffIcon();
							Icon->Init(UnitUI->CharTex, UnitUI->CharName);
							Icon->SetPadding(FMargin(0.f, 0.f, 0.f, 5.f));
							UnitList->AddChild(Icon);
							Icons.Emplace(Icon);
							UnitIconIndexs.Emplace(Unit.Unit->CharNum, cnt);
							++cnt;
						}
					}
				}
				else
				{
					GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
				}
			}), WaitTime, true);


		PlayTurnStart();
	}
}



// 평상시 PartyList Set
void UJRPGPriority::SetUnitList()
{
	Units = GM->SetUnitList;
	UnitList->ClearChildren();
	Icons.Empty();
	UnitIconIndexs.Empty();

	for(cnt = 0; cnt < Units.Num(); cnt++)
	{
		FPriorityUnit Unit = Units[cnt];
		UJRPGBattleUnitIcon* Icon = CreateWidget<UJRPGBattleUnitIcon>(GetWorld(), BP_UnitIcon);
		if (Icon)
		{
			FJRPGUnitUIStruct* UnitUI = OwnerController->GetUnitUI(Unit.Unit->CharNum);
			// 색다른 UI를 쓰려면 여기에 추가해서 사용해도 될듯.

			if (UnitUI)
			{
				Icon->Unit = Unit.Unit;
				Icon->SetDebuffIcon();
				Icon->Init(UnitUI->CharTex, UnitUI->CharName);
				Icon->SetPadding(FMargin(0.f, 0.f, 0.f, 5.f));
				UnitList->AddChild(Icon);
				Icons.Emplace(Icon);
				UnitIconIndexs.Emplace(Unit.Unit->CharNum, cnt);
			}
		}
	}

	PlayTurnStart();
}


void UJRPGPriority::PlayTurnStart()
{
	GM->TurnStart();
	FTimerHandle TurnStartHandle;
	float WaitTime = 0.5f;
	GetWorld()->GetTimerManager().SetTimer(TurnStartHandle, this, &UJRPGPriority::PlayCurrentUnit, WaitTime, false);
}


void UJRPGPriority::PlayCurrentUnit()
{
	if (Icons.IsValidIndex(0))
		Icons[0]->PlayCurrentAnim();
}


/* Priority 아이콘에 바로 버크 뜨게하기. */
void UJRPGPriority::SetupBuffIcon(int32 CharNum)
{
	if (UnitIconIndexs.Find(CharNum))
		Icons[UnitIconIndexs[CharNum]]->SetDebuffIcon();
}
