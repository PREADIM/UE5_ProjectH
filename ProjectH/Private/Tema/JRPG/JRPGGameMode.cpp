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
			}
			else // ���°�� (ù ����)
			{
				JRPGSave = Cast<UJRPGSave>(UGameplayStatics::CreateSaveGameObject(UJRPGSave::StaticClass()));
				JRPGSave->FirstSave();		
			}

			// �ڡ� JRPGSave->SetLoadCharacter(OwnerController);
			//AJRPGUnit* DefaultCharacter = GetCharacterSpawn(OwnerController->RepreCharacterNum, OwnerController->FieldLocation);

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
void AJRPGGameMode::BattleStart(int32 FieldNum, TArray<int32> Enermys)
{
	if (!GetBattleField(FieldNum)) // �ʵ� ����.
		return;

	_DEBUG("BattleStart");

	SetOwnerUnits(); // ���� ��Ʈ�ѷ��� �ִ� ��Ƽ ����Ʈ ĳ���� ����.
	SetEnermyUnits(Enermys); // �� ����.


	TurnListInit(); // ����Ʈ ��ó�� �ʱ�ȭ.

	/*if (UnitList.IsEmpty())
		return;*/

	if (SetUnitList.IsEmpty())
		return;

	// ��Ʋ�� �����ϴ� �ó׸�ƽ �����ؾ��Ѵ�. ���⼭ �����ϵ� ĳ���Ϳ� �������ɶ� �����ϵ� �Ѵ�.


	OwnerController->CameraPossess(OwnerUnits[0].Unit->GetActorLocation(), OwnerUnits[0].Unit->GetActorRotation());	// ī�޶� ��Ʈ�ѷ� ����
	//OwnerController->CameraPossess(UnitList[0].Unit->GetActorLocation(), UnitList[0].Unit->GetActorRotation());	// ī�޶� ��Ʈ�ѷ� ����
	OwnerController->DynamicCamera->CurrentField = CurrentField;

	// ���⼭ �ó׸�ƽ�� �����ؼ� �ش� �ó׸�ƽ�� �����ص� �ɵ��ϴ�. (�̱���)


	// ����������� ������ ������ �ؾ��ϴ� ������ InitAnimation�� �����Ͽ�
	// Ÿ�̸ӷ� 0.2�� �������� �ִϸ� �����Ͽ� ������ �ְ�, �� ������ �� ��ŸƮ�� �����Ѵ�.
	// �� ������ CurrentAnimation�� �����ϰ� TurnStart�� �����Ѵ�.

	OwnerController->StartBattleWidget();
}


void AJRPGGameMode::TurnStart()
{
	//���⼭ ����Ʈ ������ ĳ������ ������Ʈ�� �����Ͽ�, �ش� ĳ������ ������ ������ �ʱ�ȭ.
	/*if (UnitList.IsEmpty())
		return;*/

	if (SetUnitList.IsEmpty())
		return;

	//AJRPGUnit* Unit = UnitList[0].Unit;
	AJRPGUnit* Unit = SetUnitList[0].Unit;

	OwnerController->CurrentUnit = Unit;

	if (Unit)
	{
		//if ������ �����̻��ΰ� �Ǵ��ϱ�.��(�̱���)

		if (Unit->PlayerType == EPlayerType::Player)
		{
			Unit->OwnerUnitBattleStart();
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
	for (FPriorityUnit Units : EnermyUnits)
	{
		UnitList.HeapPush(FPriorityUnit(Units.Unit), PriorityUnitFunc());
		// Ư�� ��ġ�� ��ġ�� �ֵ��� ����.
		// ��Ʋ �ʵ带 �����ͼ� �ش� ��ġ�� �ϳ��� ����.
	}


	for (FPriorityUnit Units : OwnerUnits)
	{
		UnitList.HeapPush(FPriorityUnit(Units.Unit), PriorityUnitFunc());
		// Ư�� ��ġ�� ��ġ�� �ֵ��� ����.
		// ��Ʋ �ʵ带 �����ͼ� �ش� ��ġ�� �ϳ��� ����.
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
	/*if (UnitList.Num() > 1 && OwnerUnits.Num() > 0 && EnermyUnits.Num() > 0)
	{
		FPriorityUnit HeapTop;
		UnitList.HeapPop(HeapTop, PriorityUnitFunc());
		HeapTop.Priority = 0;
		UnitList.HeapPush(HeapTop, PriorityUnitFunc());
		if (UnitList.HeapTop().Unit->PlayerType == EPlayerType::Player)
		{
			UnitList.HeapTop().Unit->BattleStart(true); // �������� ���̰��ϰ�, �����ϴ� �Լ�.
		}
		else
		{
			UnitList.HeapTop().Unit->BattleStart(false);
		}

		//_DEBUG("Now Number : %d", UnitList.HeapTop().Unit->CharNum);
	}*/

	if (SetUnitList.Num() > 1 && OwnerUnits.Num() > 0 && EnermyUnits.Num() > 0)
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
	else
	{
		GameEnd();
	}
}



void AJRPGGameMode::GameEnd()
{

	OwnerController->UnPossess();

	for (FPriorityUnit Unit : EnermyUnits)
	{
		Unit.Unit->Destroy();
	}

	for (FPriorityUnit Unit : OwnerUnits)
	{
		Unit.Unit->Destroy();
	}



	 /*ĳ���� ���� ����*/
	/* ���� �������� ���� */
	ReturnWorld();

}


void AJRPGGameMode::ReturnWorld()
{
	// ���⼭�� �����ؼ� ���ư��� ���������ߴµ�, ��Ʋ��� �����ҋ� �׳� �ٷ� ���Ƿ� �����Ƿ�, �������ξ��ϰ� �����ص� �ɵ�.
	OwnerController->RepreCharacter = GetCharacterSpawn(OwnerController->RepreCharacterNum, OwnerController->FieldLocation);
	OwnerController->OnPossess(Cast<APawn>(OwnerController->RepreCharacter));
}

/* ���� ���� ���� ��Ʈ�ѷ����� �޾ƿ´�.*/
void AJRPGGameMode::SetOwnerUnits()
{
	if (!OwnerController)
		return;

	TArray<int32> CharList = OwnerController->CurrentParty;
	OwnerUnits.Empty();

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
				Unit->SetIsJRPGUnit(true);
				Unit->CharacterStat = OwnerController->HaveCharStat[CharList[i]];
				Unit->InitCurrentStat();
				OwnerList.Add(FLiveUnit(Unit, true));
			}
			
			OwnerUnits.HeapPush(FPriorityUnit(Unit), PriorityUnitFunc());
		}
	}
}


void AJRPGGameMode::SetEnermyUnits(TArray<int32> Enermys)
{

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

		AJRPGUnit* Unit = GetEnermySpawn(Enermys[i], UnitLocation); // ���� �ѹ��� nullptr �����

		if (Unit != nullptr)
		{
			Unit->OwnerController = OwnerController;
			//Unit->SpawnDefaultController(); // �ڡ�
			Unit->SetIsJRPGUnit(true);
			Unit->InitCurrentStat();

			EnermyUnits.HeapPush(FPriorityUnit(Unit), PriorityUnitFunc());
			EnermyList.Add(Unit); // �̰� �׳� �ʵ� ĭ ��� �ִ��� �Ǵ��ϴ� �迭.
		}

	}
}



void AJRPGGameMode::FirstStartSaveStat()
{
	// ���� ĳ���� �����. controller�� add Map�� �Ѵ�.
}


void AJRPGGameMode::SetSaveJRPG()
{
	JRPGSave->SetSave(OwnerController);
}