// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/JRPGGameMode.h"
#include "Tema/JRPG/JRPGCamera.h"
#include "Tema/JRPG/JRPGPlayerController.h"
#include "Tema/JRPG/BattleFieldList.h"
#include "Tema/JRPG/JRPGCharList.h"
#include "Tema/JRPG/BattleField.h"
#include "Tema/JRPG/JRPGSave.h"
#include "Tema/JRPG/JRPGUnit.h"
#include "Tema/JRPG/JRPGCharStat.h"
#include "Tema/JRPG/JRPGCharStatTablePaths.h"
#include "Blueprint/UserWidget.h"



FPriorityUnit::FPriorityUnit()
{
}

FPriorityUnit::FPriorityUnit(class AJRPGUnit* Char) : Unit(Char), Priority(Char->Priority)
{
}


AJRPGGameMode::AJRPGGameMode()
{
	PlayerControllerClass = AJRPGPlayerController::StaticClass();

	FString FieldDataPath = TEXT("DataTable'/Game/PROJECT/BP_CLASS/Tema/JRPG/DataBase/BattleFieldList.BattleFieldList'");
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
	}

	FString StatTable = TEXT("DataTable'/Game/PROJECT/BP_CLASS/Tema/JRPG/DataBase/JRPGStatTables.JRPGStatTables'");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_StatTable(*StatTable);
	if (DT_StatTable.Succeeded())
	{
		CharStatTablePaths = DT_StatTable.Object;
	}


	FString FieldDropTable = TEXT("DataTable'/Game/PROJECT/BP_CLASS/Tema/JRPG/DataBase/FieldEnermyDropTable.FieldEnermyDropTable'");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_FieldDropTable(*FieldDropTable);
	if (DT_FieldDropTable.Succeeded())
	{
		FieldEnermyDropTable = DT_FieldDropTable.Object;
	}

	FString IconTextureTablePath = TEXT("DataTable'/Game/PROJECT/BP_CLASS/Tema/JRPG/DataBase/DT_BuffIconStructTable.DT_BuffIconStructTable'");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_IconTextureTable(*IconTextureTablePath);
	if (DT_IconTextureTable.Succeeded())
	{
		IconTextureTable = DT_IconTextureTable.Object;
	}

	FString AttackNameAndDetailPath = TEXT("DataTable'/Game/PROJECT/BP_CLASS/Tema/JRPG/DataBase/DT_UnitAttackDetail.DT_UnitAttackDetail'");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_AttackNameAndDetail(*AttackNameAndDetailPath);
	if (DT_AttackNameAndDetail.Succeeded())
	{
		AttackNameAndDetail = DT_AttackNameAndDetail.Object;
	}

}


void AJRPGGameMode::PostLogin(APlayerController* Login)
{
	Super::PostLogin(Login);

	if (Login != nullptr)
	{
		Login->SetInputMode(FInputModeGameOnly());
		Login->SetShowMouseCursor(false);

		OwnerController = Cast<AJRPGPlayerController>(Login);
		if (OwnerController)
		{
			if (OwnerController->IsPlayerController())
			{
				OwnerController->GM = this;
				if (UGameplayStatics::DoesSaveGameExist(UJRPGSave::SlotName, 0))
				{
					JRPGSave = Cast<UJRPGSave>(UGameplayStatics::LoadGameFromSlot(UJRPGSave::SlotName, 0));
					KillCnt = JRPGSave->GetKillCnt();
				}
				else // 없는경우 (첫 시작)
				{
					JRPGSave = Cast<UJRPGSave>(UGameplayStatics::CreateSaveGameObject(UJRPGSave::StaticClass()));
					JRPGSave->FirstSave();
				}


				bBattleTutorial = JRPGSave->JRPGFieldEnermySave.bTutorial;
				bPartyTutorial = JRPGSave->JRPGFieldEnermySave.bPartyTutorial;
				SetControllerInit();
				AJRPGUnit* DefaultCharacter = GetCharacterSpawn(JRPGSave->JRPGSerial.RepreCharacterNum, JRPGSave->JRPGSerial.FieldLocation);

				if (DefaultCharacter)
					OwnerController->OnPossess(Cast<APawn>(DefaultCharacter));				
			}		
		}

	}
}

void AJRPGGameMode::SetControllerInit()
{
	JRPGSave->SetLoadCharacter(OwnerController);
	SetControllerInCharacterStat();
	SetSaveJRPG();
}


//캐릭터 스탯 테이블을 가져와서 그 테이블에서 레벨로 검색해서 스텟을 가져오기.
FJRPGCharStat AJRPGGameMode::GetCharStat(int32 CharNum, int32 Level)
{
	FJRPGCharStatTablePaths* CharStatTablePath = CharStatTablePaths->FindRow<FJRPGCharStatTablePaths>(*FString::FromInt(CharNum), TEXT(""));
	if (CharStatTablePath != nullptr)
	{
		FJRPGCharStat* Stat = CharStatTablePath->StatTable->FindRow<FJRPGCharStat>(*FString::FromInt(Level), TEXT(""));
		if (Stat)
			return *Stat;
		else
			return FJRPGCharStat();


		/* 데이터 테이블 자체가아닌 Path기반으로 런타임에서 가져오는 방법. */
		/*UDataTable* StatTable;

		// ★ 첫번째 방법.
		FSoftObjectPath Path = FSoftObjectPath(*CharStatTablePath->Path);
		StatTable = Cast<UDataTable>(Path.ResolveObject());
		if (StatTable)
		{
			FJRPGCharStat* Stat = StatTable->FindRow<FJRPGCharStat>(*FString::FromInt(Level), TEXT(""));
			return *Stat;

		}
		else
		{
			StatTable = Cast<UDataTable>(Path.TryLoad());

			if (StatTable)
			{
				FJRPGCharStat* Stat = StatTable->FindRow<FJRPGCharStat>(*FString::FromInt(Level), TEXT(""));
				return *Stat;
			}
		}
			
		return FJRPGCharStat();*/

		//★ 두번째 방법
		/*StatTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), NULL, *CharStatTablePath->Path));
		if (StatTable)
		{
			FJRPGCharStat* Stat = StatTable->FindRow<FJRPGCharStat>(*FString::FromInt(Level), TEXT(""));
			if (Stat)
				return *Stat;
			else nullptr;
		}
		else
			return FJRPGCharStat();*/
	}

	return FJRPGCharStat();	
}

void AJRPGGameMode::SetControllerInCharacterStat()
{
	for (int32 CharNum : OwnerController->HaveCharList)
	{
		FJRPGCharStat Stat = GetCharStat(CharNum, OwnerController->HaveCharLevels[CharNum]);
		OwnerController->CharStats[CharNum] = Stat;
	}
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
		{
			Unit->CharNum = CharacterNum;
			return Unit;
		}
	}	

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


FBuffIconStruct* AJRPGGameMode::GetBuffIcon(FString IconName)
{
	return IconTextureTable->FindRow<FBuffIconStruct>(*IconName, TEXT(""));
}

FJRPGUnitAttackDetail* AJRPGGameMode::GetAttackDetail(int32 CharacterNum)
{
	return AttackNameAndDetail->FindRow<FJRPGUnitAttackDetail>(*FString::FromInt(CharacterNum), TEXT(""));
}


/* 실질적인 JRPG 시작. */
bool AJRPGGameMode::BattleStart(int32 FieldNum, TArray<FEnermys> Enermys)
{
	if (!GetBattleField(FieldNum)) // 필드 생성.
		return false;

	SetUnitList.Empty();
	SetOwnerUnits(); // 오너 컨트롤러에 있는 파티 리스트 캐릭터 생성.
	SetEnermyUnits(Enermys); // 적 생성.
	TurnListInit(); // 리스트 맨처음 초기화.

	/* 카메라에 컨트롤러 빙의 */
	OwnerController->CameraPossess(OwnerUnits[0].Unit->GetActorLocation(), OwnerUnits[0].Unit->GetActorRotation(), CurrentField->AllShotLocation->GetComponentTransform());	
	if (!OwnerController->DynamicCamera)
		return false;

	float Delay = OwnerController->BattleStartSequence();

	if (SetUnitList.IsEmpty())
		return false;

	OwnerController->StartBattleWidget();
	
	FTimerHandle Timer;
	GetWorld()->GetTimerManager().SetTimer(Timer, FTimerDelegate::CreateLambda([&]()
		{
			OwnerController->bShowMouseCursor = true;
			OwnerController->BattleUIOnOff(true);
			OwnerController->PlayPriority(); // 우선순위로 짜여진 캐릭터 리스트 위젯 애님 실행.
			OwnerController->CreateBattleStartWidget();
			OwnerController->bBattleBeginning = false;
			
	}), Delay, false);

	return true;
}



void AJRPGGameMode::TurnStart()
{
	if (SetUnitList.IsEmpty())
		return;

	AJRPGUnit* Unit = SetUnitList[0].Unit;
	OwnerController->CurrentTurnUnit = Unit;

	if (Unit)
		Unit->UnitBattleStart();
}


void AJRPGGameMode::TurnEnd()
{
	TurnListSet();
}




void AJRPGGameMode::TurnListSet()
{
	//캐릭터나 적이 죽었을경우에는 죽은 캐릭터를 Find하여 지운뒤, 이 함수를 실행.
	if (OwnerList.Num() <= 0)
	{
		//여기서 게임이 끝났다는 시간을 인지하게하고 돌아가야한다.	
		float Delay = OwnerController->BattleEndSequence();
		OwnerController->SetShowMouseCursor(false);
		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(Handle, FTimerDelegate::CreateLambda([&]()
			{
				GameEnd(false); // 적이 승.
			}), Delay, false);
	}
	else if (EnermyList.Num() <= 0)
	{
		float Delay = OwnerController->BattleEndSequence();
		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(Handle, FTimerDelegate::CreateLambda([&]()
			{
				GameEnd(true); // 플레이어 승.
			}), Delay, false);
	}
	else
	{
		FPriorityUnit Unit = SetUnitList[0];
		SetUnitList.RemoveAt(0);
		SetUnitList.Emplace(Unit);
		SetUnitList[0].Unit->BattleTurnStart();
	}
}


void AJRPGGameMode::TurnListInit()
{
	// 내 캐릭터들과, 적 캐릭터들의 목록 추가하기.
	for (FPriorityUnit Units : OwnerUnits)
		UnitList.HeapPush(FPriorityUnit(Units.Unit), PriorityUnitFunc());

	for (FPriorityUnit Units : EnermyUnits)
		UnitList.HeapPush(FPriorityUnit(Units.Unit), PriorityUnitFunc());

	SetUnitListArray();

}

void AJRPGGameMode::SetUnitListArray()
{
	while (!UnitList.IsEmpty())
	{
		FPriorityUnit HeapTop;
		UnitList.HeapPop(HeapTop, PriorityUnitFunc());
		SetUnitList.Emplace(HeapTop);
	}
}


void AJRPGGameMode::WidgetCallGameEndProxy()
{
	GameEnd(false);
}

void AJRPGGameMode::GameEnd(bool bWinner)
{
	OwnerController->OnUnPossess();
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

	/* 원래 전장으로 복귀 */
	ReturnWorld();
	if (bWinner)
	{
		KillCnt++;
		OwnerController->WinGame();	
	}
	

}

void AJRPGGameMode::ReturnWorld()
{
	// 여기서는 스폰해서 돌아가는 형식으로했는데, 배틀모드 진입할떄 그냥 바로 빙의로 들어갔으므로, 스폰새로안하고 빙의해도 될듯.
	if(OwnerController->RepreCharacter == nullptr)
		OwnerController->RepreCharacter = GetCharacterSpawn(OwnerController->RepreCharacterNum, OwnerController->FieldLocation);

	OwnerController->OnPossess(Cast<APawn>(OwnerController->RepreCharacter));
	OwnerController->ReturnMainWidget();
	OwnerController->RetrunToField();
	OwnerController->bBattleING = false;
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
			if (OwnerController->HaveCharLevels.Find(CharList[i]) != nullptr)
			{
				Unit->ThisUnitBattleUnit(true);
				Unit->CharacterStat = OwnerController->CharStats[CharList[i]];
				Unit->InitCurrentStat();
				OwnerList.Emplace(Unit);
			}
			
			OwnerUnits.HeapPush(FPriorityUnit(Unit), PriorityUnitFunc());
			Unit->PlayStartMontage();
			// 레벨 스타트 몽타주 실행하기.
		}
	}
}


void AJRPGGameMode::SetEnermyUnits(TArray<FEnermys> Enermys)
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

		AJRPGUnit* Unit = GetEnermySpawn(Enermys[i].EnermyUnits, UnitLocation); // 없는 넘버는 nullptr 빈공간
		if (Unit != nullptr)
		{
			Unit->OwnerController = OwnerController;
			Unit->CharacterStat = GetCharStat(Enermys[i].EnermyUnits, Enermys[i].EnermyLevel);
			Unit->ThisUnitBattleUnit(true);
			Unit->InitCurrentStat();

			EnermyUnits.HeapPush(FPriorityUnit(Unit), PriorityUnitFunc());
			EnermyList.Emplace(Unit); // 적의 실질적인 리스트.

			Unit->PlayStartMontage();
			// 레벨 스타트 몽타주 실행하기.
		}	
	}

	int32 Index = FMath::RandRange(0, EnermyList.Num() - 1);
	if (EnermyList.IsValidIndex(Index))
		EnermyList[Index]->bPlayBattleStartSound = true;
}

void AJRPGGameMode::SetSaveBattleTutorial()
{
	JRPGSave->SetBattleTutorial();
}

void AJRPGGameMode::SetSavePartyTutorial()
{
	JRPGSave->SetPartyTutorial();
}

void AJRPGGameMode::SetSaveJRPG()
{
	JRPGSave->SetSave(OwnerController);
}

void AJRPGGameMode::SetSaveEnermyUnits(class AJRPGFieldEnermy* FieldEnermy)
{
	JRPGSave->SetFieldEnermy(FieldEnermy, KillCnt);
}

bool AJRPGGameMode::GetSaveEnermyUnits(int32 EnermyUnitNum)
{
	return JRPGSave->GetFieldEnermy(EnermyUnitNum);
}

FJRPGDropStruct AJRPGGameMode::GetFieldEnermyDropStruct(int32 EnermyUnitNum)
{
	FJRPGDropStruct* Drop = FieldEnermyDropTable->FindRow<FJRPGDropStruct>(*FString::FromInt(EnermyUnitNum), TEXT(""));

	if (Drop)
	{
		return *Drop;
	}
	else
	{
		return FJRPGDropStruct();
	}
}
