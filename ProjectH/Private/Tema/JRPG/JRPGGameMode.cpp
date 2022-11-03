// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/JRPGGameMode.h"
#include "Tema/JRPG/JRPGCamera.h"
#include "Tema/JRPG/JRPGPlayerController.h"
#include "Tema/JRPG/BattleFieldList.h"
#include "Tema/JRPG/JRPGCharList.h"
#include "Tema/JRPG/BattleField.h"
#include "Tema/JRPG/JRPGComponent.h"
#include "Tema/JRPG/JRPGSave.h"
#include "Tema/JRPG/JRPGUnit.h"
#include "Tema/JRPG/JRPGEnermy.h"


FLiveUnit::FLiveUnit()
{
}

FLiveUnit::FLiveUnit(AJRPGUnit* U, bool b) : Unit(U), bLive(b)
{
}



FPriorityUnit::FPriorityUnit()
{
}

FPriorityUnit::FPriorityUnit(class AJRPGUnit* Char) : Unit(Char), Priority(Char->Priority)
{
}




AJRPGGameMode::AJRPGGameMode()
{
	PlayerControllerClass = AJRPGPlayerController::StaticClass();

	SetDataTable(FieldTable, TEXT("DataTable'/Game/PROJECT/BP_CLASS/Tema/JRPG/DataBase/BattleFieldList.BattleFieldList'"));
	SetDataTable(CharListTable, TEXT("DataTable'/Game/PROJECT/BP_CLASS/Tema/JRPG/DataBase/JRPGCharList.JRPGCharList'"));
	SetDataTable(EnermyListTable, TEXT("DataTable'/Game/PROJECT/BP_CLASS/Tema/JRPG/DataBase/EnermyList.EnermyList'"));





	/*FString FieldDataPath = TEXT("DataTable'/Game/PROJECT/BP_CLASS/Tema/JRPG/DataBase/BattleFieldList.BattleFieldList'");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_FieldData(*FieldDataPath);
	if (DT_FieldData.Succeeded())
	{
		FieldTable = DT_FieldData.Object;
		
	}

	FString CharListDataPath = TEXT("DataTable'/Game/PROJECT/BP_CLASS/Tema/JRPG/DataBase/JRPGCharList.JRPGCharList'");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_ListData(*CharListDataPath);
	if (DT_ListData.Succeeded())
	{
		CharListTable = DT_ListData.Object;
	}

	FString EnermyListDataPath = TEXT("DataTable'/Game/PROJECT/BP_CLASS/Tema/JRPG/DataBase/EnermyList.EnermyList'");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_EnermyListData(*EnermyListDataPath);
	if (DT_EnermyListData.Succeeded())
	{
		EnermyListTable = DT_EnermyListData.Object;
	}*/

}

void AJRPGGameMode::SetDataTable(UDataTable* Table, FString TablePath)
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Table(*TablePath);
	if (DT_Table.Succeeded())
	{
		Table = DT_Table.Object;
	}
}


void AJRPGGameMode::PostLogin(APlayerController* Login)
{
	Super::PostLogin(Login);

	if (Login != nullptr)
	{
		OwnerController = Cast<AJRPGPlayerController>(Login);
		if (OwnerController->IsPlayerController())
		{
			OwnerController->GM = this;
			if (UGameplayStatics::DoesSaveGameExist(UJRPGSave::SlotName, 0))
			{
				JRPGSave = Cast<UJRPGSave>(UGameplayStatics::LoadGameFromSlot(UJRPGSave::SlotName, 0));
				_DEBUG("Load");
				KillCnt = JRPGSave->GetKillCnt();
			}
			else // 없는경우 (첫 시작)
			{
				JRPGSave = Cast<UJRPGSave>(UGameplayStatics::CreateSaveGameObject(UJRPGSave::StaticClass()));
				JRPGSave->FirstSave();		
			}


			AJRPGUnit* DefaultCharacter = GetCharacterSpawn(JRPGSave->JRPGSerial.RepreCharacterNum, JRPGSave->JRPGSerial.FieldLocation);

			if (DefaultCharacter)
			{
				OwnerController->OnPossess(Cast<APawn>(DefaultCharacter));
			}
			SetSaveJRPG();
		}

	}
}

void AJRPGGameMode::SetControllerInit()
{
	JRPGSave->SetLoadCharacter(OwnerController);
	
}

/*캐릭터 검색해서 해당 캐릭터 스폰하기.*/
AJRPGUnit* AJRPGGameMode::GetCharacterSpawn(int32 CharacterNum, FTransform UnitLocation)
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	FJRPGCharList* List = CharListTable->FindRow<FJRPGCharList>(*FString::FromInt(CharacterNum), TEXT(""));
	if (List)
	{
		AJRPGUnit* Unit = GetWorld()->SpawnActor<class AJRPGUnit>(List->BP_JRPGCharacter, UnitLocation, SpawnParameters);
		if (Unit)
			Unit->CharNum = CharacterNum;
		return Unit;
	}	
	else
		return nullptr;
}


/*적 캐릭터를 검색해서 해당 캐릭터 스폰하기.*/
AJRPGUnit* AJRPGGameMode::GetEnermySpawn(int32 CharacterNum, FTransform UnitLocation)
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	FJRPGCharList* List = EnermyListTable->FindRow<FJRPGCharList>(*FString::FromInt(CharacterNum), TEXT(""));
	if (List)
	{
		AJRPGUnit* Unit = GetWorld()->SpawnActor<class AJRPGUnit>(List->BP_JRPGCharacter, UnitLocation, SpawnParameters);
		if (Unit)
			Unit->CharNum = CharacterNum;
		return Unit;
	}	
	else
		return nullptr;
}


/* 배틀 필드 가져오기. */
bool AJRPGGameMode::GetBattleField(int32 FieldNum)
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	FBattleFieldList* List = FieldTable->FindRow<FBattleFieldList>(*FString::FromInt(FieldNum), TEXT(""));
	if (CurrentField != nullptr)
		CurrentField->Destroy();

	CurrentField = GetWorld()->SpawnActor<class ABattleField>(List->BP_BattleField, FTransform(List->SpawnLocation), SpawnParameters);

	if (CurrentField)
		return true;
	else
		return false;
}


/* 실질적인 JRPG 시작. */
void AJRPGGameMode::BattleStart(int32 FieldNum, TArray<int32> Enermys)
{
	if (!GetBattleField(FieldNum)) // 필드 생성.
		return;

	_DEBUG("BattleStart");

	SetUnitList.Empty();
	SetOwnerUnits(); // 오너 컨트롤러에 있는 파티 리스트 캐릭터 생성.
	SetEnermyUnits(Enermys); // 적 생성.


	TurnListInit(); // 리스트 맨처음 초기화.

	/*if (UnitList.IsEmpty())
		return;*/

	if (SetUnitList.IsEmpty())
		return;

	// 배틀에 진입하는 시네마틱 구현해야한다. 여기서 실행하든 캐릭터와 오버랩될때 시작하든 한다.


	OwnerController->CameraPossess(OwnerUnits[0].Unit->GetActorLocation(), OwnerUnits[0].Unit->GetActorRotation());	// 카메라에 컨트롤러 빙의
	//OwnerController->CameraPossess(UnitList[0].Unit->GetActorLocation(), UnitList[0].Unit->GetActorRotation());	// 카메라에 컨트롤러 빙의
	OwnerController->DynamicCamera->CurrentField = CurrentField;
	OwnerController->GameType = EGameModeType::Battle;

	// 여기서 시네마틱도 전달해서 해당 시네마틱을 실행해도 될듯하다. (미구현)


	OwnerController->StartBattleWidget();
}


void AJRPGGameMode::TurnStart()
{
	//여기서 리스트 맨위의 캐릭터의 컴포넌트에 접근하여, 해당 캐릭터의 정보를 위젯에 초기화.
	/*if (UnitList.IsEmpty())
		return;*/

	if (SetUnitList.IsEmpty())
		return;

	//AJRPGUnit* Unit = UnitList[0].Unit;
	AJRPGUnit* Unit = SetUnitList[0].Unit;

	OwnerController->CurrentUnit = Unit;

	if (Unit)
	{
		//if 유닛이 상태이상인가 판단하기.★(미구현)

		if (Unit->PlayerType == EPlayerType::Player)
		{
			Unit->OwnerUnitBattleStart();
		}
		else // 적의 차례
		{
			// 적이 타겟팅한 캐릭터에 카메라가 위치해 있는다.
			Unit->EnermyBattleStart();
		}
	}
}


void AJRPGGameMode::TurnEnd()
{
	TurnListSet();
}


void AJRPGGameMode::TurnListInit()
{
	// 내 캐릭터들과, 적 캐릭터들의 목록 추가하기.
	for (FPriorityUnit Units : EnermyUnits)
	{
		UnitList.HeapPush(FPriorityUnit(Units.Unit), PriorityUnitFunc());
	}


	for (FPriorityUnit Units : OwnerUnits)
	{
		UnitList.HeapPush(FPriorityUnit(Units.Unit), PriorityUnitFunc());
	}

	SetUnitListArray();

}

void AJRPGGameMode::SetUnitListArray()
{
	while (!UnitList.IsEmpty())
	{
		FPriorityUnit HeapTop;
		UnitList.HeapPop(HeapTop, PriorityUnitFunc());
		SetUnitList.Add(HeapTop);
	}
}


void AJRPGGameMode::TurnListSet()
{
	// ★★ 캐릭터나 적이 죽었을경우에는 죽은 캐릭터를 Find하여 지운뒤, 이 함수를 실행.

	if (OwnerList.Num() <= 0)
	{
		//여기서 게임이 끝났다는 시간을 인지하게하고 돌아가야한다.
		GameEnd(false); // 적이 승.
	}
	else if(EnermyList.Num() <= 0)
	{
		//여기서 게임이 끝났다는 시간을 인지하게하고 돌아가야한다.
		GameEnd(true); // 플레이어 승.
	}
	else
	{
		FPriorityUnit Unit = SetUnitList[0];
		SetUnitList.RemoveAt(0);
		SetUnitList.Add(Unit);

		if (SetUnitList[0].Unit->PlayerType == EPlayerType::Player)
		{
			Unit.Unit->BattleStart(true);
		}
		else
		{
			Unit.Unit->BattleStart(false);
		}
	}
}



void AJRPGGameMode::GameEnd(bool bWinner)
{
	OwnerController->UnPossess();

	for (FPriorityUnit& Unit : EnermyUnits)
	{
		if(IsValid(Unit.Unit))
			Unit.Unit->Destroy();
	}

	for (FPriorityUnit& Unit : OwnerUnits)
	{
		if (IsValid(Unit.Unit))
			Unit.Unit->Destroy();
	}

	 /*캐릭터 전부 삭제*/
	/* 원래 전장으로 복귀 */
	ReturnWorld();

	if (bWinner)
	{
		KillCnt++;
		CurrentBattleEnermy->FieldEnermyDead(); // 플레이어가 승리.	
	}

}


void AJRPGGameMode::ReturnWorld()
{
	// 여기서는 스폰해서 돌아가는 형식으로했는데, 배틀모드 진입할떄 그냥 바로 빙의로 들어갔으므로, 스폰새로안하고 빙의해도 될듯.
	if(OwnerController->RepreCharacter == nullptr)
		OwnerController->RepreCharacter = GetCharacterSpawn(OwnerController->RepreCharacterNum, OwnerController->FieldLocation);

	OwnerController->OnPossess(Cast<APawn>(OwnerController->RepreCharacter));
	OwnerController->ReturnMainWidget();

}

/* 오너 유닛 생성 컨트롤러에서 받아온다.*/
void AJRPGGameMode::SetOwnerUnits()
{
	if (!OwnerController)
		return;

	TArray<int32> CharList = OwnerController->CurrentParty;
	OwnerUnits.Empty();
	OwnerList.Empty();

	for (int32 i = 0; i < CharList.Num(); i++)
	{
		FTransform UnitLocation;
		switch (i)
		{
		case 0:
			UnitLocation = CurrentField->Unit1->GetComponentTransform();
			break;
		case 1:
			UnitLocation = CurrentField->Unit2->GetComponentTransform();
			break;
		case 2:
			UnitLocation = CurrentField->Unit3->GetComponentTransform();
			break;
		default:
			break;
		}

		AJRPGUnit* Unit = GetCharacterSpawn(CharList[i], UnitLocation);
		if (Unit != nullptr)
		{
			Unit->OwnerController = OwnerController;
		
			if (OwnerController->HaveCharStat.Find(CharList[i]) != nullptr)
			{
				Unit->ThisUnitBattleUnit(true);
				Unit->CharacterStat = OwnerController->HaveCharStat[CharList[i]];
				Unit->InitCurrentStat();
				OwnerList.Add(Unit);
			}
			
			OwnerUnits.HeapPush(FPriorityUnit(Unit), PriorityUnitFunc());
		}
	}
}


void AJRPGGameMode::SetEnermyUnits(TArray<int32> Enermys)
{
	EnermyUnits.Empty();
	EnermyList.Empty();

	for (int32 i = 0; i < Enermys.Num(); i++)
	{
		FTransform UnitLocation;
		switch (i)
		{
		case 0:
			UnitLocation = CurrentField->Enermy1->GetComponentTransform();
			break;
		case 1:
			UnitLocation = CurrentField->Enermy2->GetComponentTransform();
			break;
		case 2:
			UnitLocation = CurrentField->Enermy3->GetComponentTransform();
			break;
		default:
			break;
		}

		AJRPGUnit* Unit = GetEnermySpawn(Enermys[i], UnitLocation); // 없는 넘버는 nullptr 빈공간

		if (Unit != nullptr)
		{
			Unit->OwnerController = OwnerController;
			Unit->ThisUnitBattleUnit(true);
			Unit->InitCurrentStat();

			EnermyUnits.HeapPush(FPriorityUnit(Unit), PriorityUnitFunc());
			EnermyList.Add(Unit); // 적의 실질적인 리스트.
		}

	}
}



void AJRPGGameMode::SetSaveJRPG()
{
	JRPGSave->SetSave(OwnerController);
}



void AJRPGGameMode::SetSaveEnermyUnits(class AJRPGEnermy* FieldEnermy)
{
	JRPGSave->SetFieldEnermy(FieldEnermy, KillCnt);
}

bool AJRPGGameMode::GetSaveEnermyUnits(int32 EnermyUnitNum)
{
	return JRPGSave->GetFieldEnermy(EnermyUnitNum);
}
