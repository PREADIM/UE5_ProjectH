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
				else // ���°�� (ù ����)
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


//ĳ���� ���� ���̺��� �����ͼ� �� ���̺��� ������ �˻��ؼ� ������ ��������.
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


		/* ������ ���̺� ��ü���ƴ� Path������� ��Ÿ�ӿ��� �������� ���. */
		/*UDataTable* StatTable;

		// �� ù��° ���.
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

		//�� �ι�° ���
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


/*ĳ���� �˻��ؼ� �ش� ĳ���� �����ϱ�.*/
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


/*�� ĳ���͸� �˻��ؼ� �ش� ĳ���� �����ϱ�.*/
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


/* ��Ʋ �ʵ� ��������. */
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


/* �������� JRPG ����. */
bool AJRPGGameMode::BattleStart(int32 FieldNum, TArray<FEnermys> Enermys)
{
	if (!GetBattleField(FieldNum)) // �ʵ� ����.
		return false;

	SetUnitList.Empty();
	SetOwnerUnits(); // ���� ��Ʈ�ѷ��� �ִ� ��Ƽ ����Ʈ ĳ���� ����.
	SetEnermyUnits(Enermys); // �� ����.
	TurnListInit(); // ����Ʈ ��ó�� �ʱ�ȭ.

	/* ī�޶� ��Ʈ�ѷ� ���� */
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
			OwnerController->PlayPriority(); // �켱������ ¥���� ĳ���� ����Ʈ ���� �ִ� ����.
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
	//ĳ���ͳ� ���� �׾�����쿡�� ���� ĳ���͸� Find�Ͽ� �����, �� �Լ��� ����.
	if (OwnerList.Num() <= 0)
	{
		//���⼭ ������ �����ٴ� �ð��� �����ϰ��ϰ� ���ư����Ѵ�.	
		float Delay = OwnerController->BattleEndSequence();
		OwnerController->SetShowMouseCursor(false);
		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(Handle, FTimerDelegate::CreateLambda([&]()
			{
				GameEnd(false); // ���� ��.
			}), Delay, false);
	}
	else if (EnermyList.Num() <= 0)
	{
		float Delay = OwnerController->BattleEndSequence();
		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(Handle, FTimerDelegate::CreateLambda([&]()
			{
				GameEnd(true); // �÷��̾� ��.
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
	// �� ĳ���͵��, �� ĳ���͵��� ��� �߰��ϱ�.
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

	/* ���� �������� ���� */
	ReturnWorld();
	if (bWinner)
	{
		KillCnt++;
		OwnerController->WinGame();	
	}
	

}

void AJRPGGameMode::ReturnWorld()
{
	// ���⼭�� �����ؼ� ���ư��� ���������ߴµ�, ��Ʋ��� �����ҋ� �׳� �ٷ� ���Ƿ� �����Ƿ�, �������ξ��ϰ� �����ص� �ɵ�.
	if(OwnerController->RepreCharacter == nullptr)
		OwnerController->RepreCharacter = GetCharacterSpawn(OwnerController->RepreCharacterNum, OwnerController->FieldLocation);

	OwnerController->OnPossess(Cast<APawn>(OwnerController->RepreCharacter));
	OwnerController->ReturnMainWidget();
	OwnerController->RetrunToField();
	OwnerController->bBattleING = false;
}

/* ���� ���� ���� ��Ʈ�ѷ����� �޾ƿ´�.*/
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
			// ���� ��ŸƮ ��Ÿ�� �����ϱ�.
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

		AJRPGUnit* Unit = GetEnermySpawn(Enermys[i].EnermyUnits, UnitLocation); // ���� �ѹ��� nullptr �����
		if (Unit != nullptr)
		{
			Unit->OwnerController = OwnerController;
			Unit->CharacterStat = GetCharStat(Enermys[i].EnermyUnits, Enermys[i].EnermyLevel);
			Unit->ThisUnitBattleUnit(true);
			Unit->InitCurrentStat();

			EnermyUnits.HeapPush(FPriorityUnit(Unit), PriorityUnitFunc());
			EnermyList.Emplace(Unit); // ���� �������� ����Ʈ.

			Unit->PlayStartMontage();
			// ���� ��ŸƮ ��Ÿ�� �����ϱ�.
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
