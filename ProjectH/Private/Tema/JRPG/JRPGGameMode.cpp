// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/JRPGGameMode.h"
#include "Tema/JRPG/JRPGPlayerController.h"
#include "Tema/JRPG/BattleFieldList.h"
#include "Tema/JRPG/JRPGCharList.h"
#include "Tema/JRPG/BattleField.h"
#include "Tema/JRPG/JRPGComponent.h"
#include "Tema/JRPG/JRPGSave.h"


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
			}
			else // ���°�� (ù ����)
			{
				JRPGSave = Cast<UJRPGSave>(UGameplayStatics::CreateSaveGameObject(UJRPGSave::StaticClass()));
			}
			JRPGSave->SetLoadCharacter(OwnerController);
			//���⼭ �ش� ĳ������ ���̺� ������ �����ͼ� ��Ʈ�ѷ��� �������ش�.
			AJRPGUnit* DefaultCharacter = GetCharacterSpawn(OwnerController->RepreCharacterNum, OwnerController->FieldLocation);
			if (DefaultCharacter)
			{
				OwnerController->OnPossess(Cast<APawn>(DefaultCharacter));
			}
		}

	}
}

/*ĳ���� �˻��ؼ� �ش� ĳ���� �����ϱ�.*/
AJRPGUnit* AJRPGGameMode::GetCharacterSpawn(int32 CharacterNum, FTransform UnitLocation)
{
	FJRPGCharList* List = CharListTable->FindRow<FJRPGCharList>(*FString::FromInt(CharacterNum), TEXT(""));
	if (List)
		return GetWorld()->SpawnActor<class AJRPGUnit>(List->BP_JRPGCharacter, UnitLocation);
	else
		return nullptr;
}


/*�� ĳ���͸� �˻��ؼ� �ش� ĳ���� �����ϱ�.*/
AJRPGUnit* AJRPGGameMode::GetEnermySpawn(int32 CharacterNum, FTransform UnitLocation)
{
	FJRPGCharList* List = EnermyListTable->FindRow<FJRPGCharList>(*FString::FromInt(CharacterNum), TEXT(""));
	if (List)
		return GetWorld()->SpawnActor<class AJRPGUnit>(List->BP_JRPGCharacter, UnitLocation);
	else
		return nullptr;
}


/* ��Ʋ �ʵ� ��������. */
bool AJRPGGameMode::GetBattleField(int32 FieldNum)
{
	FBattleFieldList* List = FieldTable->FindRow<FBattleFieldList>(*FString::FromInt(FieldNum), TEXT(""));
	CurrentField = GetWorld()->SpawnActor<class ABattleField>(List->BP_BattleField, FTransform(List->SpawnLocation));
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


	OwnerController->CameraPossess(FVector::ZeroVector);	// ī�޶� ��Ʈ�ѷ� ����
	// ��ó�� ��ġ�� ���߿� ��Ʋ �ʵ带 ����� �׳� �� �� ����� ����.
	// ���⼭ �ó׸�ƽ�� �����ؼ� �ش� �ó׸�ƽ�� �����ص� �ɵ��ϴ�. (�̱���)


	//�ڡ�(�̱���) OwnerController->SetViewTargetWithBlend(OwnerController, 1.0f);
	// ��ó���� ��Ʋ�ʵ��� ����ī�޶� ���ٰ� �� ���ۿ� ���̳��� ī�޶�� ���� �ϱ�.

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
			Unit->BattleStart();
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

	for (int32 i = 0; i < 3; i++)
	{
		if (CharList.IsValidIndex(i))
		{
			FTransform UnitLocation;
			switch (i)
			{
			case 0:
				UnitLocation = CurrentField->Unit1->GetComponentTransform();
				break;
			case 1:
				UnitLocation = CurrentField->Unit1->GetComponentTransform();
				break;
			case 2:
				UnitLocation = CurrentField->Unit1->GetComponentTransform();
				break;
			default:
				break;
			}
			
			OwnerUnits[i] = GetCharacterSpawn(CharList[i], UnitLocation); // ���� �ѹ��� nullptr �����
			OwnerUnits[i]->OwnerController = OwnerController;
			OwnerUnits[i]->BattleComponent->SetOwnerPCAndGM(OwnerController, this);
		}
			
	}

}


void AJRPGGameMode::SetEnermyUnits(TArray<int32> Enermys)
{

	for (int32 i = 0; i < 3; i++)
	{
		if (Enermys.IsValidIndex(i))
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

			EnermyUnits[i] = GetEnermySpawn(Enermys[i], UnitLocation); // ���� �ѹ��� nullptr �����
			EnermyUnits[i]->OwnerController = OwnerController;
			EnermyUnits[i]->BattleComponent->SetOwnerPCAndGM(OwnerController, this);
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