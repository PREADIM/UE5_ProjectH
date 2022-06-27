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
			else // 없는경우 (첫 시작)
			{
				JRPGSave = Cast<UJRPGSave>(UGameplayStatics::CreateSaveGameObject(UJRPGSave::StaticClass()));
			}
			JRPGSave->SetLoadCharacter(OwnerController);
			//여기서 해당 캐릭터의 세이브 파일을 가져와서 컨트롤러에 대입해준다.
			AJRPGUnit* DefaultCharacter = GetCharacterSpawn(OwnerController->RepreCharacterNum, OwnerController->FieldLocation);
			if (DefaultCharacter)
			{
				OwnerController->OnPossess(Cast<APawn>(DefaultCharacter));
			}
		}

	}
}

/*캐릭터 검색해서 해당 캐릭터 스폰하기.*/
AJRPGUnit* AJRPGGameMode::GetCharacterSpawn(int32 CharacterNum, FTransform UnitLocation)
{
	FJRPGCharList* List = CharListTable->FindRow<FJRPGCharList>(*FString::FromInt(CharacterNum), TEXT(""));
	if (List)
		return GetWorld()->SpawnActor<class AJRPGUnit>(List->BP_JRPGCharacter, UnitLocation);
	else
		return nullptr;
}


/*적 캐릭터를 검색해서 해당 캐릭터 스폰하기.*/
AJRPGUnit* AJRPGGameMode::GetEnermySpawn(int32 CharacterNum, FTransform UnitLocation)
{
	FJRPGCharList* List = EnermyListTable->FindRow<FJRPGCharList>(*FString::FromInt(CharacterNum), TEXT(""));
	if (List)
		return GetWorld()->SpawnActor<class AJRPGUnit>(List->BP_JRPGCharacter, UnitLocation);
	else
		return nullptr;
}


/* 배틀 필드 가져오기. */
bool AJRPGGameMode::GetBattleField(int32 FieldNum)
{
	FBattleFieldList* List = FieldTable->FindRow<FBattleFieldList>(*FString::FromInt(FieldNum), TEXT(""));
	CurrentField = GetWorld()->SpawnActor<class ABattleField>(List->BP_BattleField, FTransform(List->SpawnLocation));
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

	SetOwnerUnits(); // 오너 컨트롤러에 있는 파티 리스트 캐릭터 생성.
	SetEnermyUnits(Enermys); // 적 생성.


	TurnListInit(); // 리스트 맨처음 초기화.

	if (UnitList.IsEmpty())
		return;

	// 배틀에 진입하는 시네마틱 구현해야한다. 여기서 실행하든 캐릭터와 오버랩될때 시작하든 한다.


	OwnerController->CameraPossess(FVector::ZeroVector);	// 카메라에 컨트롤러 빙의
	// 맨처음 위치는 나중에 배틀 필드를 만들면 그냥 그 한 가운데로 하자.
	// 여기서 시네마틱도 전달해서 해당 시네마틱을 실행해도 될듯하다. (미구현)


	//★★(미구현) OwnerController->SetViewTargetWithBlend(OwnerController, 1.0f);
	// 맨처음엔 배틀필드의 메인카메라를 보다가 턴 시작에 다이나믹 카메라로 블렌드 하기.

	TurnStart();

}


void AJRPGGameMode::TurnStart()
{
	//여기서 리스트 맨위의 캐릭터의 컴포넌트에 접근하여, 해당 캐릭터의 정보를 위젯에 초기화.
	if (UnitList.IsEmpty())
		return;

	AJRPGUnit* Unit = UnitList[0].Unit;
	if (Unit)
	{
		//if 유닛이 상태이상인가 판단하기.★(미구현)

		if (Unit->PlayerType == EPlayerType::Player)
		{
			OwnerController->CameraSetUp(Unit->GetActorLocation());
			Unit->BattleStart();
		}
		else // 적의 차례
		{
			// 적이 타겟팅한 캐릭터에 카메라가 위치해 있는다.
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
	// 내 캐릭터들과, 적 캐릭터들의 목록 추가하기.
	for (AJRPGUnit* Units : EnermyUnits)
	{
		UnitList.HeapPush(FPriorityUnit(Units), PriorityUnitFunc());
		// 특정 위치에 위치해 있도록 설정.
		// 배틀 필드를 가져와서 해당 위치에 하나씩 놓자.
	}


	for (AJRPGUnit* Units : OwnerUnits)
	{
		UnitList.HeapPush(FPriorityUnit(Units), PriorityUnitFunc());
		// 특정 위치에 위치해 있도록 설정.
		// 배틀 필드를 가져와서 해당 위치에 하나씩 놓자.
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



	 /*캐릭터 전부 삭제*/
	/* 원래 전장으로 복귀 */
	ReturnWorld();

}


void AJRPGGameMode::ReturnWorld()
{
	OwnerController->RepreCharacter = GetCharacterSpawn(OwnerController->RepreCharacterNum, OwnerController->FieldLocation);
	OwnerController->OnPossess(Cast<APawn>(OwnerController->RepreCharacter));
}

/* 오너 유닛 생성 컨트롤러에서 받아온다.*/
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
			
			OwnerUnits[i] = GetCharacterSpawn(CharList[i], UnitLocation); // 없는 넘버는 nullptr 빈공간
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

			EnermyUnits[i] = GetEnermySpawn(Enermys[i], UnitLocation); // 없는 넘버는 nullptr 빈공간
			EnermyUnits[i]->OwnerController = OwnerController;
			EnermyUnits[i]->BattleComponent->SetOwnerPCAndGM(OwnerController, this);
		}

	}
}



void AJRPGGameMode::FirstStartSaveStat()
{
	// 기초 캐릭터 저장식. controller에 add Map을 한다.
}


void AJRPGGameMode::SetSaveJRPG()
{
	JRPGSave->SetSave(OwnerController);
}