// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/JRPGGameMode.h"
#include "Tema/JRPG/JRPGPlayerController.h"
#include "Tema/JRPG/BattleFieldList.h"
#include "Tema/JRPG/JRPGCharList.h"
#include "Tema/JRPG/BattleField.h"
#include "Tema/JRPG/JRPGComponent.h"
#include "Tema/JRPG/JRPGSave.h"
#include "Tema/JRPG/JRPGUnit.h"


FPriorityUnit::FPriorityUnit()
{

}

FPriorityUnit::FPriorityUnit(class AJRPGUnit* Char)
{
	Unit = Char;
	Priority = Char->Priority;
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
		return GetWorld()->SpawnActor<class AJRPGUnit>(List->BP_JRPGCharacter, UnitLocation, SpawnParameters);
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
		return GetWorld()->SpawnActor<class AJRPGUnit>(List->BP_JRPGCharacter, UnitLocation, SpawnParameters);
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

	SetOwnerUnits(); // ���� ��Ʈ�ѷ��� �ִ� ��Ƽ ����Ʈ ĳ���� ����.
	SetEnermyUnits(Enermys); // �� ����.


	TurnListInit(); // ����Ʈ ��ó�� �ʱ�ȭ.

	if (UnitList.IsEmpty())
		return;

	// ��Ʋ�� �����ϴ� �ó׸�ƽ �����ؾ��Ѵ�. ���⼭ �����ϵ� ĳ���Ϳ� �������ɶ� �����ϵ� �Ѵ�.


	OwnerController->CameraPossess(OwnerUnits[0]->GetActorLocation(), OwnerUnits[0]->GetActorRotation());	// ī�޶� ��Ʈ�ѷ� ����
	//OwnerController->CameraPossess(UnitList[0].Unit->GetActorLocation(), UnitList[0].Unit->GetActorRotation());	// ī�޶� ��Ʈ�ѷ� ����
	OwnerController->SetShowMouseCursor(true);
	OwnerController->SetInputMode(FInputModeUIOnly());


	// ���⼭ �ó׸�ƽ�� �����ؼ� �ش� �ó׸�ƽ�� �����ص� �ɵ��ϴ�. (�̱���)
	TurnStart();

}


void AJRPGGameMode::TurnStart()
{
	//���⼭ ����Ʈ ������ ĳ������ ������Ʈ�� �����Ͽ�, �ش� ĳ������ ������ ������ �ʱ�ȭ.
	if (UnitList.IsEmpty())
		return;

	AJRPGUnit* Unit = UnitList[0].Unit;
	if (Unit)
	{
		//if ������ �����̻��ΰ� �Ǵ��ϱ�.��(�̱���)

		if (Unit->PlayerType == EPlayerType::Player)
		{
			OwnerController->CameraSetUp(Unit->GetActorLocation());
			Unit->BattleStart();
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
	TurnStart();
}


void AJRPGGameMode::TurnListInit()
{
	// �� ĳ���͵��, �� ĳ���͵��� ��� �߰��ϱ�.
	for (AJRPGUnit* Units : EnermyUnits)
	{
		UnitList.HeapPush(FPriorityUnit(Units), PriorityUnitFunc());
		// Ư�� ��ġ�� ��ġ�� �ֵ��� ����.
		// ��Ʋ �ʵ带 �����ͼ� �ش� ��ġ�� �ϳ��� ����.
	}


	for (AJRPGUnit* Units : OwnerUnits)
	{
		UnitList.HeapPush(FPriorityUnit(Units), PriorityUnitFunc());
		// Ư�� ��ġ�� ��ġ�� �ֵ��� ����.
		// ��Ʋ �ʵ带 �����ͼ� �ش� ��ġ�� �ϳ��� ����.
	}

	for (FPriorityUnit UnitLists : UnitList)
	{
		_DEBUG("%d", UnitLists.Priority);
	}

}


void AJRPGGameMode::TurnListSet()
{
	UnitList.Add(FPriorityUnit(UnitList[0]));
	UnitList.RemoveAt(0);

}



void AJRPGGameMode::GameEnd()
{

	OwnerController->UnPossess();

	for (AJRPGUnit* Unit : EnermyUnits)
	{
		Unit->Destroy();
	}

	for (AJRPGUnit* Unit : OwnerUnits)
	{
		Unit->Destroy();
	}



	 /*ĳ���� ���� ����*/
	/* ���� �������� ���� */
	ReturnWorld();

}


void AJRPGGameMode::ReturnWorld()
{
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
			Unit->BattleComponent->SetOwnerPCAndGM(OwnerController, this);
			if (OwnerController->HaveCharStat.Find(CharList[i]) != nullptr)
			{
				Unit->CharacterStat = OwnerController->HaveCharStat[CharList[i]];
			}
			
			OwnerUnits.Add(Unit);
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
			Unit->BattleComponent->SetOwnerPCAndGM(OwnerController, this);
			EnermyUnits.Add(Unit);
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