// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/BattleUI/JRPGPriority.h"
#include "Tema/JRPG/BattleUI/JRPGBattleUnitIcon.h"
#include "Tema/JRPG/JRPGPlayerController.h"
#include "Components/VerticalBox.h"


void UJRPGPriority::PlayInit()
{
	/*if (BP_UnitIcon && GM)
	{	
		float WaitTime = 0.2f;
		Units = GM->UnitList;
		UnitList->ClearChildren();
		Icons.Empty();

		GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			if (!Units.IsEmpty())
			{
				FPriorityUnit HeapTop;
				Units.HeapPop(HeapTop, PriorityUnitFunc());
				UJRPGBattleUnitIcon* Icon = CreateWidget<UJRPGBattleUnitIcon>(GetWorld(), BP_UnitIcon);
				if (Icon)
				{
					FJRPGUnitUIStruct* UnitUI = OwnerController->GetUnitUI(HeapTop.Unit->CharNum);
					// ���ٸ� UI�� ������ ���⿡ �߰��ؼ� ����ص� �ɵ�.

					if (UnitUI)
					{
						Icon->Init(UnitUI->CharTex, UnitUI->CharName);
						Icon->SetPadding(FMargin(0.f, 0.f, 0.f, 5.f));
						UnitList->AddChild(Icon);
						Icons.Add(Icon);
					}
				}
			}
			else
			{					
				GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
				_DEBUG("Clear");
			}
		}), WaitTime, true);


		PlayTurnStart();
	
	}*/

	if (BP_UnitIcon && GM)
	{
		float WaitTime = 0.2f;
		Units = GM->SetUnitList;
		UnitList->ClearChildren();
		Icons.Empty();
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
						// ���ٸ� UI�� ������ ���⿡ �߰��ؼ� ����ص� �ɵ�.

						if (UnitUI)
						{
							Icon->Init(UnitUI->CharTex, UnitUI->CharName);
							Icon->SetPadding(FMargin(0.f, 0.f, 0.f, 5.f));
							UnitList->AddChild(Icon);
							Icons.Add(Icon);

							++cnt;
						}
					}
				}
				else
				{
					GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
					_DEBUG("Clear");
				}
			}), WaitTime, true);


		PlayTurnStart();

	}


}



// ���� PartyList Set
void UJRPGPriority::SetUnitList()
{
	/*Units = GM->UnitList;
	UnitList->ClearChildren();
	Icons.Empty();

	while (!Units.IsEmpty())
	{
		FPriorityUnit HeapTop;
		Units.HeapPop(HeapTop, PriorityUnitFunc());
		UJRPGBattleUnitIcon* Icon = CreateWidget<UJRPGBattleUnitIcon>(GetWorld(), BP_UnitIcon);
		if (Icon)
		{
			FJRPGUnitUIStruct* UnitUI = OwnerController->GetUnitUI(HeapTop.Unit->CharNum);
			// ���ٸ� UI�� ������ ���⿡ �߰��ؼ� ����ص� �ɵ�.

			if (UnitUI)
			{
				Icon->Init(UnitUI->CharTex, UnitUI->CharName);
				Icon->SetPadding(FMargin(0.f, 0.f, 0.f, 5.f));
				UnitList->AddChild(Icon);
				Icons.Add(Icon);
			}
		}
	}

	PlayTurnStart();*/


	Units = GM->SetUnitList;
	UnitList->ClearChildren();
	Icons.Empty();

	for(cnt = 0; cnt < Units.Num(); cnt++)
	{
		FPriorityUnit Unit = Units[cnt];
		UJRPGBattleUnitIcon* Icon = CreateWidget<UJRPGBattleUnitIcon>(GetWorld(), BP_UnitIcon);
		if (Icon)
		{
			FJRPGUnitUIStruct* UnitUI = OwnerController->GetUnitUI(Unit.Unit->CharNum);
			// ���ٸ� UI�� ������ ���⿡ �߰��ؼ� ����ص� �ɵ�.

			if (UnitUI)
			{
				Icon->Init(UnitUI->CharTex, UnitUI->CharName);
				Icon->SetPadding(FMargin(0.f, 0.f, 0.f, 5.f));
				UnitList->AddChild(Icon);
				Icons.Add(Icon);
			}
		}
	}

	PlayTurnStart();
}


void UJRPGPriority::PlayTurnStart()
{
	GM->TurnStart();
	FTimerHandle TurnStartHandle;
	float WaitTime = 1.0f;
	GetWorld()->GetTimerManager().SetTimer(TurnStartHandle, this, &UJRPGPriority::PlayCurrentUnit, WaitTime, false);
}


void UJRPGPriority::PlayCurrentUnit()
{
	if (Icons.IsValidIndex(0))
	{
		Icons[0]->PlayCurrentAnim();	
	}
}



