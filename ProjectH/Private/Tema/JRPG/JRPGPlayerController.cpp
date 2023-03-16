// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/JRPGPlayerController.h"
#include "Tema/JRPG/JRPGCamera.h"
#include "Tema/JRPG/JRPGGameMode.h"
#include "Tema/JRPG/JRPGEnermy.h"
#include "Tema/JRPG/MainUI/JRPGTemaUI.h"
#include "Kismet/KismetMathLibrary.h"
#include "Tema/JRPG/MainUI/PartySettingField.h"
#include "Tema/JRPG/CustomWidget.h"
#include "Tema/JRPG/BattleUI/DamageWidget.h"
#include "Tema/JRPG/JRPGUnit.h"
#include <LevelSequencePlayer.h>
#include <LevelSequenceActor.h>
#include <MovieSceneSequencePlayer.h>
#include "Tema/JRPG/BattleUI/DropItemWidget.h"
#include "Tema/JRPG/BattleUI/DropExpWidget.h"
#include "UI/Custom/CAWTextAnimWidget.h"



AJRPGPlayerController::AJRPGPlayerController()
{

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));

	FString UnitUIDataPath = TEXT("DataTable'/Game/PROJECT/BP_CLASS/Tema/JRPG/DataBase/JRPGUnitUIState.JRPGUnitUIState'");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_UnitUI(*UnitUIDataPath);
	if (DT_UnitUI.Succeeded())
	{
		UnitUITable = DT_UnitUI.Object;
	}
}

void AJRPGPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (BP_TemaMainUI)
	{
		TemaMainUI = CreateWidget<UJRPGTemaUI>(GetWorld(), BP_TemaMainUI);
		TemaMainUI->OwnerController = this;
		TemaMainUI->GM = GM;
		TemaMainUI->Init();
		TemaMainUI->AddToViewport();
	}

	if (BP_DropItemWidget && BP_DropExpWidget)
	{
		DropCharWidget = CreateWidget<UDropItemWidget>(GetWorld(), BP_DropItemWidget);
		DropExpWidget = CreateWidget<UDropExpWidget>(GetWorld(), BP_DropExpWidget);

		DropCharWidget->OwnerController = this;
	}

	if (NormalSound)
		SetPlaySound(NormalSound);
}

void AJRPGPlayerController::OnPossess(APawn* NewPawn)
{
	Super::OnPossess(NewPawn);

	if (Cast<AJRPGUnit>(NewPawn))
	{
		RepreCharacter = Cast<AJRPGUnit>(NewPawn);
		RepreCharacterNum = RepreCharacter->CharNum;

		if (GM)
			GM->SetControllerInit();

	}	
}


void AJRPGPlayerController::OnUnPossess()
{
	Super::OnUnPossess();
}


/* ĳ���Ϳ� �ش��ϴ� ��Ƽ ������, UI�� �� ĳ���� �̸��� ����Ǿ� �ִ� ��.*/
FJRPGUnitUIStruct* AJRPGPlayerController::GetUnitUI(int32 CharacterNum)
{
	FJRPGUnitUIStruct* UnitUI = UnitUITable->FindRow<FJRPGUnitUIStruct>(*FString::FromInt(CharacterNum), TEXT(""));
	if (UnitUI)
		return UnitUI;
	else
		return nullptr;
}


/* ��Ƽ������ �� �� ������ */
UTexture2D* AJRPGPlayerController::GetPartySettingIcon(int32 CharacterNum)
{
	FJRPGUnitUIStruct* UnitUI = UnitUITable->FindRow<FJRPGUnitUIStruct>(*FString::FromInt(CharacterNum), TEXT(""));
	if (UnitUI)
		return UnitUI->PartySettingIcon;
	else
		return nullptr;
}


void AJRPGPlayerController::CameraPossess(FVector Location, FRotator Rotation)
{
	if (BP_Camera)
	{
		DynamicCamera = GetWorld()->SpawnActor<AJRPGCamera>(BP_Camera, FTransform(Location));
		DynamicCamera->TargetLocation = Location;
		DynamicCamera->TargetRotation = Rotation;	
		OnUnPossess();
		OnPossess(DynamicCamera);
		DynamicCamera->OwnerController = this;
		SetShowMouseCursor(true);
		SetInputMode(FInputModeGameAndUI());
		// ���̳��� ī�޶� ���ǵȰŸ� ��Ʋ ������ �Ǿ��ٴ� ��.
	}
}


void AJRPGPlayerController::CameraSetUp(FVector Location)
{
	if(DynamicCamera)
		DynamicCamera->TargetLocation = Location;
}

void AJRPGPlayerController::CameraRotSetUp(FRotator Rotation)
{
	if (DynamicCamera)
		DynamicCamera->TargetRotation = Rotation;
}

void AJRPGPlayerController::ExitCamera()
{
	OnUnPossess();
	OnPossess(Cast<APawn>(RepreCharacter));
}


FVector AJRPGPlayerController::GetCameraLocation()
{
	return DynamicCamera->TargetLocation;
}


void AJRPGPlayerController::AddDropChar(int32 CharNum)
{
	if (HaveCharList.Find(CharNum) == INDEX_NONE)
	{
		HaveCharList.Emplace(CharNum);
		HaveCharLevels.Emplace(CharNum, 1);
		CharStats.Emplace(CharNum, GetCharStat(CharNum));
		CurrentExp.Emplace(CharNum, 0.0f);
	}
	SetSave();
}

void AJRPGPlayerController::DropItem()
{
	if (CurrentOverlapFieldEnermy)
	{
		FJRPGDropStruct DropStruct = CurrentOverlapFieldEnermy->DropStruct;

		for (int32 CharNum : CurrentParty)
			AddCharExp(CharNum, DropStruct.DropExp);

		SetupDropExpWidget(DropStruct.DropExp);

		if (DropStruct.DropCharNum != 0)
		{
			SetupDropCharWidget(DropStruct.DropCharNum);

			if (!GM->bPartyTutorial) // ĳ���� ��������� ��Ƽ ���� �غ���� Ʃ�丮�� ����.
				PartyTutorialStart();
		}
	}
}


void AJRPGPlayerController::SetupDropCharWidget(int32 DropCharNum)
{
	if (DropCharWidget)
	{
		AddDropChar(DropCharNum);
		FString DropCharName = GetUnitUI(DropCharNum)->CharName;
		DropCharWidget->Init(DropCharName);
		DropCharWidget->AddToViewport();
	}

}


void AJRPGPlayerController::SetupDropExpWidget(int32 DropExp)
{
	if (DropExpWidget)
	{
		DropExpWidget->Init(DropExp);
		DropExpWidget->AddToViewport();
	}
}

void AJRPGPlayerController::SetPlaySound(USoundBase* Sound)
{
	if (!AudioComponent)
		return;

	if (!Sound)
		AudioComponent->SetSound(NormalSound);
	else
	{
		AudioComponent->SetSound(Sound);
	}
	AudioComponent->Play();
}

/* ��Ʋ �����Ҷ� �̰��� ����. */
bool AJRPGPlayerController::PlayBattleMode(AJRPGFieldEnermy* CurrentFieldEnermy)
{
	if (GM)
	{
		if (RepreCharacter)
		{
			CurrentOverlapFieldEnermy = CurrentFieldEnermy;
			FieldLocation = RepreCharacter->GetActorTransform();
			GM->SetSaveJRPG();
			TemaMainUI->StartBattleWidget();		
		}
		bBattleING = GM->BattleStart(CurrentFieldNum, CurrentOverlapFieldEnermy->EnermyUnits);
		bBattleBeginning = bBattleING;
		if (bBattleING)
			CurrentOverlapFieldEnermy->BattleStart();

		return bBattleING;
	}
	return false;
}

void AJRPGPlayerController::ReturnMainWidget()
{
	TemaMainUI->StartMainWidget();
}

void AJRPGPlayerController::WinGame()
{
	DropItem();
	CurrentOverlapFieldEnermy->FieldEnermyDead();
}


void AJRPGPlayerController::RetrunToField()
{
	SetPlaySound(NormalSound);
	CurrentOverlapFieldEnermy->ReturnToField();
}


void AJRPGPlayerController::GameEndSpawnCharacter()
{
	TemaMainUI->StartMainWidget();
}



void AJRPGPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction(TEXT("ESC"), IE_Released, this, &AJRPGPlayerController::OpenESC);
	// �ӽ÷� PŰ�� ������.
	InputComponent->BindAction(TEXT("MouseOnOff"), IE_Released, this, &AJRPGPlayerController::MouseOnOff);

	InputComponent->BindAction(TEXT("1"), IE_Released, this, &AJRPGPlayerController::SetParty_First);
	InputComponent->BindAction(TEXT("2"), IE_Released, this, &AJRPGPlayerController::SetParty_Second);
	InputComponent->BindAction(TEXT("3"), IE_Released, this, &AJRPGPlayerController::SetParty_Third);

}



void AJRPGPlayerController::OpenESC()
{
	if (TemaMainUI)
	{
		if (TemaMainUI->MainIsInViewport())
		{
			if (!LastWidget.IsEmpty())
			{
				LastWidget.Top()->SetCloseFunction();
				LastWidget.Pop();
			}
			else
				TemaMainUI->OpenESCMenu();

		}
		else if (TemaMainUI->BattleIsInViewport())
			BattleESC();
	}
}


void AJRPGPlayerController::MouseOnOff()
{
	
	if (bShowMouseCursor)
	{
		MouseOff();
	}
	else
	{
		MouseOn();
	}
}


void AJRPGPlayerController::MouseOn()
{
	int32 x, y;
	GetViewportSize(x, y);
	SetMouseLocation(UKismetMathLibrary::FTrunc(x / 2), UKismetMathLibrary::FTrunc(y / 2));
	SetShowMouseCursor(true);
	SetInputMode(FInputModeGameAndUI());
}


void AJRPGPlayerController::MouseOff()
{
	SetShowMouseCursor(false);
	SetInputMode(FInputModeGameOnly());
}

void AJRPGPlayerController::SetSave()
{
	if (GM)
	{
		GM->SetSaveJRPG();
	}
}


void AJRPGPlayerController::SetPartyChange()
{
	if (TemaMainUI)
	{
		TemaMainUI->SetPartyChange();
	}
}

void AJRPGPlayerController::SetParty_First()
{
	SetRepreCharacterSpawn(0);
}

void AJRPGPlayerController::SetParty_Second()
{
	SetRepreCharacterSpawn(1);
}

void AJRPGPlayerController::SetParty_Third()
{
	SetRepreCharacterSpawn(2);
}



void AJRPGPlayerController::SetRepreCharacterSpawn(int32 index)
{
	if (CurrentParty.IsValidIndex(index) && CurrentPartyIndex != index)
	{
		FRotator Rot = GetControlRotation();
		FVector Loc = RepreCharacter->GetActorLocation();
		AJRPGUnit* Unit = GM->GetCharacterSpawn(CurrentParty[index], RepreCharacter->GetActorTransform());
		RepreCharacter->Destroy();
		RepreCharacter = Unit;
		OnPossess(RepreCharacter);
		RepreCharacter->SetActorLocation(Loc);
		SetControlRotation(Rot);
		CurrentPartyIndex = index;

		RepreCharacter ->PlayCharacterChangeMontage();
	}
}




void AJRPGPlayerController::SetRepreCharacterSpawnUI(int32 index)
{
	if (CurrentParty.IsValidIndex(index))
	{
		if (RepreCharacterNum != CurrentParty[index])
		{
			FVector Loc = RepreCharacter->GetActorLocation();
			AJRPGUnit* Unit = GM->GetCharacterSpawn(CurrentParty[index], RepreCharacter->GetActorTransform());
			RepreCharacter->Destroy();
			RepreCharacter = Unit;
			OnPossess(RepreCharacter);
			RepreCharacter->SetActorLocation(Loc);
			CurrentPartyIndex = index;
		}
		else
		{
			OnPossess(RepreCharacter);
		}
		
	}
}

FJRPGCharStat AJRPGPlayerController::GetCharStat(int32 CharNum)
{
	return GM->GetCharStat(CharNum, HaveCharLevels[CharNum]);
}

void AJRPGPlayerController::AddCharExp(int32 CharNum, float DropExp)
{
	float Exp = CurrentExp[CharNum] + DropExp;

	if (HaveCharLevels[CharNum] == 5)
		return;

	if (Exp >= CharStats[CharNum].NextEXP)
	{
		CurrentExp[CharNum] = Exp - CharStats[CharNum].NextEXP; // ���� ����ġ �ʱ�ȭ
		HaveCharLevels[CharNum] += 1; // ���� ����
		CharStats[CharNum] = GetCharStat(CharNum); // ���� ������
	}
	else
	{
		CurrentExp[CharNum] = Exp;
	}

	GM->SetSaveJRPG();
}


// ��Ʋ ���� �� ó�� �����ִϸ��̼� ȿ���ֱ�
void AJRPGPlayerController::StartBattleWidget()
{
	if (TemaMainUI)
		TemaMainUI->PlayBattleWidget();
}


// �Ͻ��۽� ���� �����ϱ�.
void AJRPGPlayerController::BattleTurnStart(bool bPlayer)
{
	TemaMainUI->BattleTurnStart(bPlayer);
}


/* ��Ʋ �� ������ ���� */
void AJRPGPlayerController::BattleESC()
{
	if (BP_BattleESCWidget)
	{
		if (BattleESCWidget)
		{
			if (BattleESCWidget->IsInViewport())
			{
				BattleESCWidget->RemoveFromParent();
				return;
			}			
		}

		BattleESCWidget = CreateWidget<UCustomAnimWidget>(GetWorld(), BP_BattleESCWidget);
		if (BattleESCWidget)
		{
			BattleESCWidget->AddToViewport();
			BattleESCWidget->PlayCustomAnimation();
		}		
	}
}



void AJRPGPlayerController::VisibleDamage(float Damage, FVector TargetLocation)
{
	UDamageWidget* DamageSkin = nullptr;

	if (BP_DamageSkin)
	{
		DamageSkin = CreateWidget<UDamageWidget>(GetWorld(), BP_DamageSkin);
		if (DamageSkin != nullptr)
		{
			DamageSkin->OwnerController = this;
			DamageSkin->Init(Damage, TargetLocation);
		}
	}
}



void AJRPGPlayerController::UnitTurnEnd()
{
	if (GM)
		GM->TurnEnd();
}



/*--------------------------------------------------
	PlayerController->TemaMainUI->BattlWidget�� �������̽�
---------------------------------------------------*/

// ���� �����ܰ� BattleWidget�� Visible ó��
void AJRPGPlayerController::SetVisibleBattleWidget(bool bFlag)
{
	TemaMainUI->SetVisibleBattleWidget(bFlag);
}


// ���� �����϶��� BattleWidget�� ��������, ��ų�°� �� ����â�� ���̸� �ȵȴ�.
void AJRPGPlayerController::SetEnermyTurnWidget(bool bFlag)
{
	TemaMainUI->SetEnermyTurnWidget(bFlag);
}



void AJRPGPlayerController::EnermyListSetup()
{
	TemaMainUI->EnermyListSetup();
}

void AJRPGPlayerController::HiddenLockOn()
{
	TemaMainUI->HiddenLockOn();
}

void AJRPGPlayerController::EnermySetupLockOnTargetUnit(AJRPGUnit* Target)
{
	//TemaMainUI->EnermySetupLockOnTargetUnit(Target);
	TargetUnit = Target;
}



void AJRPGPlayerController::TargetToRotation()
{
	TemaMainUI->TargetToRotation();
}


void AJRPGPlayerController::EnermyTargetToRotation()
{
	TemaMainUI->EnermyTargetToRotation();
}

float AJRPGPlayerController::BattleStartSequence()
{
	SequencePlayer = nullptr;

	ALevelSequenceActor* LQActor;
	if (StartSequence)
		SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), StartSequence, FMovieSceneSequencePlaybackSettings(), LQActor);

	if (SequencePlayer)
	{
		BattleUIOnOff(false);
		SequencePlayer->Play();
		SetPlaySound(BattleStartSound);	

		FTimerHandle Handle;
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUFunction(this, FName("SetPlaySound"), BattleSound);
		GetWorld()->GetTimerManager().SetTimer(Handle, TimerDelegate, 9.f, false);

		return SequencePlayer->GetEndTime().AsSeconds();
	}

	return 0.0f;
}

float AJRPGPlayerController::BattleEndSequence()
{
	SequencePlayer = nullptr;

	ALevelSequenceActor* LQActor;
	if (EndSequence)
		SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), EndSequence, FMovieSceneSequencePlaybackSettings(), LQActor);

	if (SequencePlayer)
	{
		BattleUIOnOff(false);
		SequencePlayer->Play();
		return SequencePlayer->GetEndTime().AsSeconds();
	}

	return 0.0f;
}


// �� �Լ��� �ƿ� ��� ������ ���� Ű�� ���̴�.
void AJRPGPlayerController::BattleUIOnOff(bool bOnOff)
{
	TemaMainUI->BattleUIOnOff(bOnOff);
	for (FPriorityUnit Unit : GM->SetUnitList)
		Unit.Unit->BattleWidgetOnOff(bOnOff);
}


void AJRPGPlayerController::PlayPriority()
{
	TemaMainUI->PlayPriority();
}


void AJRPGPlayerController::CreateBattleStartWidget()
{
	if (BP_BattleStartWidget)
	{
		UCustomAnimWidget* BattleStartWidget = CreateWidget<UCustomAnimWidget>(GetWorld(), BP_BattleStartWidget);
		if (BattleStartWidget)
		{
			BattleStartWidget->AddToViewport();
			BattleStartWidget->PlayCustomAnimation();
		}
	}
}


void AJRPGPlayerController::SetTurnEndDebuffWidget(ECCType LastCCType)
{
	if (!BP_TurnEndCCStateWidget)
		return;

	UCAWTextAnimWidget* TurnEndCCStateWidget = CreateWidget<UCAWTextAnimWidget>(GetWorld(), BP_TurnEndCCStateWidget);
	if (TurnEndCCStateWidget)
	{
		switch (LastCCType)
		{
		case ECCType::STUN:
			TurnEndCCStateWidget->SetCAWText("S T U N");
			break;
		}

		TurnEndCCStateWidget->AddToViewport();
		TurnEndCCStateWidget->PlayCustomAnimation();
	}
	
}




void AJRPGPlayerController::BattleTutorialStart()
{
	if (BP_BattleTutorialWidget)
	{
		UUserWidget* BattleTT = CreateWidget<UUserWidget>(GetWorld(), BP_BattleTutorialWidget);
		if (BattleTT)
		{
			BattleTT->AddToViewport();
			GM->bBattleTutorial = true;
			GM->SetSaveBattleTutorial();		
		}
	}
}

void AJRPGPlayerController::PartyTutorialStart()
{
	if (BP_PartyTutorialWidget)
	{
		UUserWidget* PartyTT = CreateWidget<UUserWidget>(GetWorld(), BP_PartyTutorialWidget);
		if (PartyTT)
		{
			PartyTT->AddToViewport();
			GM->bPartyTutorial = true;
			GM->SetSavePartyTutorial();
		}
	}
}