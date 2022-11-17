// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/JRPGGameMode.h"
#include "Tema/JRPG/JRPGCamera.h"
#include "Tema/JRPG/JRPGPlayerController.h"
#include "Tema/JRPG/BattleFieldList.h"
#include "Tema/JRPG/JRPGCharList.h"
#include "Tema/JRPG/BattleField.h"
#include "Tema/JRPG/JRPGComponent.h"
#include "Tema/JRPG/JRPGSave.h"
#include "Tema/JRPG/JRPGEnermy.h"
#include "Tema/JRPG/JRPGUnit.h"
#include "Tema/JRPG/JRPGCharStat.h"
#include "Tema/JRPG/JRPGCharStatTablePaths.h"
#include "Blueprint/UserWidget.h"


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


	/*SetDataTable(FieldTable, FString(TEXT("DataTable'/Game/PROJECT/BP_CLASS/Tema/JRPG/DataBase/BattleFieldList.BattleFieldList'")));
	SetDataTable(CharListTable, FString(TEXT("DataTable'/Game/PROJECT/BP_CLASS/Tema/JRPG/DataBase/JRPGCharList.JRPGCharList'")));
	SetDataTable(EnermyListTable, FString(TEXT("DataTable'/Game/PROJECT/BP_CLASS/Tema/JRPG/DataBase/EnermyList.EnermyList'")));
	SetDataTable(CharStatTablePaths, FString(TEXT("DataTable'/Game/PROJECT/BP_CLASS/Tema/JRPG/DataBase/JRPGCharStatTablePaths.JRPGCharStatTablePaths'")));
	SetDataTable(FieldEnermyDropTable, FString(TEXT("DataTable'/Game/PROJECT/BP_CLASS/Tema/JRPG/DataBase/FieldEnermyDropTable.FieldEnermyDropTable'")));
	*/

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

	FString StatTable = TEXT("DataTable'/Game/PROJECT/BP_CLASS/Tema/JRPG/DataBase/JRPGCharStatTablePaths.JRPGCharStatTablePaths'");
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

}

void AJRPGGameMode::SetDataTable(UDataTable*& Table, FString TablePath)
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
				KillCnt = JRPGSave->GetKillCnt();
			}
			else // ���°�� (ù ����)
			{
				JRPGSave = Cast<UJRPGSave>(UGameplayStatics::CreateSaveGameObject(UJRPGSave::StaticClass()));
				JRPGSave->FirstSave();		

			}

			bBattleTutorial = JRPGSave->JRPGFieldEnermy.bTutorial;
			bPartyTutorial = JRPGSave->JRPGFieldEnermy.bPartyTutorial;
			AJRPGUnit* DefaultCharacter = GetCharacterSpawn(JRPGSave->JRPGSerial.RepreCharacterNum, JRPGSave->JRPGSerial.FieldLocation);

			if (DefaultCharacter)
			{
				OwnerController->OnPossess(Cast<APawn>(DefaultCharacter));

				SetCurrentExpAndNextExp();
			}

			SetSaveJRPG();
		}

	}
}

void AJRPGGameMode::SetControllerInit()
{
	JRPGSave->SetLoadCharacter(OwnerController);
	
}


// �ڡ� ĳ���� ���� ���̺��� �����ͼ� �� ���̺��� ������ �˻��ؼ� ������ ��������.
FJRPGCharStat AJRPGGameMode::GetCharStat(int32 CharNum, int32 Level)
{
	FJRPGCharStatTablePaths* CharStatTablePath = CharStatTablePaths->FindRow<FJRPGCharStatTablePaths>(*FString::FromInt(CharNum), TEXT(""));
	if (CharStatTablePath != nullptr)
	{
		// �ڡڡ� FObjectFinder�� COD������ �����ϴ�.
		/*static ConstructorHelpers::FObjectFinder<UDataTable> DT_Table(*CharStatTablePath->Path);
		if (DT_Table.Succeeded())
		{
			StatTable = DT_Table.Object;
		}*/

		//�ڡ� ������ ���̺� ��Ÿ�ӿ��� �������� ���.
		UDataTable* StatTable;

		// �� ù��° ���.
		/*FSoftObjectPath Path = FSoftObjectPath(*CharStatTablePath->Path);
		StatTable = Cast<UDataTable>(Path.ResolveObject());
		if (StatTable)
		{
			FJRPGCharStat* Stat = StatTable->FindRow<FJRPGCharStat>(*FString::FromInt(Level), TEXT(""));
			return *Stat;
		}
		else
			StatTable = Cast<UDataTable>(Path.TryLoad());

		if (StatTable)
		{
			FJRPGCharStat* Stat = StatTable->FindRow<FJRPGCharStat>(*FString::FromInt(Level), TEXT(""));
			return *Stat;
		}

		return FJRPGCharStat();*/

		//�� �ι�° ���
		StatTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), NULL, *CharStatTablePath->Path));
		if (StatTable)
		{
			FJRPGCharStat* Stat = StatTable->FindRow<FJRPGCharStat>(*FString::FromInt(Level), TEXT(""));
			return *Stat;
		}
		else
			return FJRPGCharStat();
	}


	return FJRPGCharStat();	
}

void AJRPGGameMode::SetCurrentExpAndNextExp()
{
	for (int32 CharNum : OwnerController->HaveCharList)
	{
		FJRPGCharStat Stat = GetCharStat(CharNum, OwnerController->HaveCharLevels[CharNum]);
		OwnerController->CharStats[CharNum] = Stat;
		OwnerController->NextExp[CharNum] = Stat.NextEXP;
	}

	// �ڡڡ� NextExp�� CharStats�� ���̺긦 �����ʰ�, ���� ���۽� ������ ���̺��� ������ ���Ȱ����� �����Ѵ�.
	// ���� ���� ���� �Ǵ� ���� ����ؾ��ϱ� ����. �̹� ����Ǿ������� �����Ͱ� �̻�������.
	// �ڡ� ������ ��ó�� Add�� ���� Ű���� �����ؾ��ϴ� ���� �ؾ��ϹǷ� ���̺� ��ü�� �صд�.

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
			Unit->CharNum = CharacterNum;
		return Unit;
	}	
	else
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


/* �������� JRPG ����. */
void AJRPGGameMode::BattleStart(int32 FieldNum, TArray<FEnermys> Enermys)
{
	if (!GetBattleField(FieldNum)) // �ʵ� ����.
		return;

	SetUnitList.Empty();
	SetOwnerUnits(); // ���� ��Ʈ�ѷ��� �ִ� ��Ƽ ����Ʈ ĳ���� ����.
	SetEnermyUnits(Enermys); // �� ����.
	TurnListInit(); // ����Ʈ ��ó�� �ʱ�ȭ.


	OwnerController->CameraPossess(OwnerUnits[0].Unit->GetActorLocation(), OwnerUnits[0].Unit->GetActorRotation());	// ī�޶� ��Ʈ�ѷ� ����
	OwnerController->DynamicCamera->CurrentField = CurrentField;
	OwnerController->GameType = EGameModeType::Battle;

	float Delay = OwnerController->BattleStartSequence();

	if (SetUnitList.IsEmpty())
		return;

	OwnerController->StartBattleWidget();

	FTimerHandle Timer;
	GetWorld()->GetTimerManager().SetTimer(Timer, FTimerDelegate::CreateLambda([&]()
	{
			OwnerController->BattleUIOnOff(true);
			OwnerController->PlayPriority(); // �켱������ ¥���� ĳ���� ����Ʈ ���� �ִ� ����.				
	}), Delay, false);
}


void AJRPGGameMode::TurnStart()
{
	//���⼭ ����Ʈ ������ ĳ������ ������Ʈ�� �����Ͽ�, �ش� ĳ������ ������ ������ �ʱ�ȭ.

	if (SetUnitList.IsEmpty())
		return;

	AJRPGUnit* Unit = SetUnitList[0].Unit;

	OwnerController->CurrentUnit = Unit;

	if (Unit)
	{
		//if ������ �����̻��ΰ� �Ǵ��ϱ�.��(�̱���)

		if (Unit->PlayerType == EPlayerType::Player)
		{
			Unit->OwnerUnitBattleStart();
			if (!bBattleTutorial) // Ʃ�丮�� ����
			{
				BattleTutorialStart();
			}
		}
		else // ���� ����
		{
			// ���� Ÿ������ ĳ���Ϳ� ī�޶� ��ġ�� �ִ´�.
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
	// �� ĳ���͵��, �� ĳ���͵��� ��� �߰��ϱ�.

	for (FPriorityUnit Units : OwnerUnits)
	{
		UnitList.HeapPush(FPriorityUnit(Units.Unit), PriorityUnitFunc());
	}


	for (FPriorityUnit Units : EnermyUnits)
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
	// �ڡ� ĳ���ͳ� ���� �׾�����쿡�� ���� ĳ���͸� Find�Ͽ� �����, �� �Լ��� ����.

	if (OwnerList.Num() <= 0)
	{
		//���⼭ ������ �����ٴ� �ð��� �����ϰ��ϰ� ���ư����Ѵ�.	
		float Delay = OwnerController->BattleEndSequence();
		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(Handle, FTimerDelegate::CreateLambda([&]()
		{

			GameEnd(false); // ���� ��.
		}), Delay, false);
	}
	else if(EnermyList.Num() <= 0)
	{
		//���⼭ ������ �����ٴ� �ð��� �����ϰ��ϰ� ���ư����Ѵ�.
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

	 /*ĳ���� ���� ����*/
	/* ���� �������� ���� */
	ReturnWorld();

	if (bWinner)
	{
		KillCnt++;
		CurrentBattleEnermy->FieldEnermyDead(); // �÷��̾ �¸�.	
	}

}


void AJRPGGameMode::ReturnWorld()
{
	// ���⼭�� �����ؼ� ���ư��� ���������ߴµ�, ��Ʋ��� �����ҋ� �׳� �ٷ� ���Ƿ� �����Ƿ�, �������ξ��ϰ� �����ص� �ɵ�.
	if(OwnerController->RepreCharacter == nullptr)
		OwnerController->RepreCharacter = GetCharacterSpawn(OwnerController->RepreCharacterNum, OwnerController->FieldLocation);

	OwnerController->OnPossess(Cast<APawn>(OwnerController->RepreCharacter));
	OwnerController->ReturnMainWidget();

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
				OwnerList.Add(Unit);
			}
			
			OwnerUnits.HeapPush(FPriorityUnit(Unit), PriorityUnitFunc());
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
			Unit->EnermyLevel = Enermys[i].EnermyLevel;
			Unit->CharacterStat = GetCharStat(Enermys[i].EnermyUnits, Enermys[i].EnermyLevel);
			Unit->ThisUnitBattleUnit(true);
			Unit->InitCurrentStat();

			EnermyUnits.HeapPush(FPriorityUnit(Unit), PriorityUnitFunc());
			EnermyList.Add(Unit); // ���� �������� ����Ʈ.
		}

	}
}



void AJRPGGameMode::BattleTutorialStart()
{
	if (BP_BattleTutorialWidget)
	{
		UUserWidget* BattleTT = CreateWidget<UUserWidget>(GetWorld(), BP_BattleTutorialWidget);
		if (BattleTT)
		{
			BattleTT->AddToViewport();
			bBattleTutorial = true;
			JRPGSave->SetBattleTutorial();
		}
	}
}

void AJRPGGameMode::PartyTutorialStart()
{
	if (BP_PartyTutorialWidget)
	{
		UUserWidget* PartyTT = CreateWidget<UUserWidget>(GetWorld(), BP_PartyTutorialWidget);
		if (PartyTT)
		{
			PartyTT->AddToViewport();
			bPartyTutorial = true;
			JRPGSave->SetPartyTutorial();
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
