// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/JRPGPlayerController.h"
#include "Tema/JRPG/JRPGCamera.h"
#include "Tema/JRPG/JRPGGameMode.h"
#include "Tema/JRPG/MainUI/JRPGTemaUI.h"
#include "Kismet/KismetMathLibrary.h"
#include "Tema/JRPG/MainUI/PartySettingField.h"
#include "Tema/JRPG/CustomWidget.h"


AJRPGPlayerController::AJRPGPlayerController()
{
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

	GameType = EGameModeType::Normal;
}

void AJRPGPlayerController::OnPossess(APawn* NewPawn)
{
	Super::OnPossess(NewPawn);

	if (Cast<AJRPGUnit>(NewPawn))
	{
		RepreCharacter = Cast<AJRPGUnit>(NewPawn);

		// ★★ 원래는 이미 있는 RepreCharacter로 HaveCharStat에서 받아오는 것이지만, 나중에 구현.

		GM->SetControllerInit(); // OnPossess를 하면 왜인지는 모르겠으나, 값이 초기화된다. 그래서 다시 설정.
	}	
}


/* 캐릭터에 해당하는 파티 프로필, UI에 들어갈 캐릭터 이름이 저장되어 있는 곳.*/
FJRPGUnitUIStruct* AJRPGPlayerController::GetUnitUI(int32 CharacterNum)
{
	FJRPGUnitUIStruct* UnitUI = UnitUITable->FindRow<FJRPGUnitUIStruct>(*FString::FromInt(CharacterNum), TEXT(""));
	if (UnitUI)
		return UnitUI;
	else
		return nullptr;
}


/* 파티세팅할 때 뜰 아이콘 */
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
		UnPossess();
		OnPossess(DynamicCamera);
		// 다이나믹 카메라가 빙의된거면 배틀 시작이 되었다는 뜻.
	}
}


void AJRPGPlayerController::CameraSetUp(FVector Location)
{
	if(DynamicCamera)
		DynamicCamera->SetActorLocation(Location);
}

void AJRPGPlayerController::ExitCamera()
{
	UnPossess();
	OnPossess(Cast<APawn>(RepreCharacter));
}


/* 유닛에서 공격을 받던 공격을 하던 배틀 시작할때 이것을 실행. */
void AJRPGPlayerController::PlayBattleMode(TArray<int32> EnermyUnits)
{
	// 에너미 유닛은 추후에 캐릭터가 공격했을때 닿은 적이 가지고있는 것으르 가져온다.
	if (GM)
	{
		if (RepreCharacter)
		{
			FieldLocation = RepreCharacter->GetActorTransform();
			GM->SetSaveJRPG();

			TemaMainUI->StartBattleWidget();		
		}
		GM->BattleStart(CurrentFieldNum, EnermyUnits);
	}
}


void AJRPGPlayerController::GameEndSpawnCharacter()
{
	TemaMainUI->StartMainWidget();
}



void AJRPGPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction(TEXT("ESC"), IE_Released, this, &AJRPGPlayerController::OpenESC);
	// 임시로 P키에 지정함.
	InputComponent->BindAction(TEXT("MouseOnOff"), IE_Released, this, &AJRPGPlayerController::MouseOnOff);
	//InputComponent->BindAction(TEXT("LMB"), IE_Released, this, &AJRPGPlayerController::LMB);


}



void AJRPGPlayerController::OpenESC()
{
	if (TemaMainUI)
	{
		if (GameType == EGameModeType::Normal)
			TemaMainUI->OpenESCMenu();
		else if (GameType == EGameModeType::Battle)
			TemaMainUI->OpenBattleESCMenu();
		else if (GameType == EGameModeType::UI)
		{
			if (!LastWidget.IsEmpty())
			{
				LastWidget.Top()->SetCloseFunction();
				if (LastWidget.IsEmpty()) // 이제서야 다 비워졌다면
				{
					GameType = EGameModeType::Normal;
				}
			}
			else
			{
				TemaMainUI->CloseESCMenu();
			}

		}
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