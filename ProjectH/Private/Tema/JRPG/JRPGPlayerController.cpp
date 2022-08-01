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

		// �ڡ� ������ �̹� �ִ� RepreCharacter�� HaveCharStat���� �޾ƿ��� ��������, ���߿� ����.

		GM->SetControllerInit(); // OnPossess�� �ϸ� �������� �𸣰�����, ���� �ʱ�ȭ�ȴ�. �׷��� �ٽ� ����.
	}	
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
		UnPossess();
		OnPossess(DynamicCamera);
		// ���̳��� ī�޶� ���ǵȰŸ� ��Ʋ ������ �Ǿ��ٴ� ��.
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


/* ���ֿ��� ������ �޴� ������ �ϴ� ��Ʋ �����Ҷ� �̰��� ����. */
void AJRPGPlayerController::PlayBattleMode(TArray<int32> EnermyUnits)
{
	// ���ʹ� ������ ���Ŀ� ĳ���Ͱ� ���������� ���� ���� �������ִ� ������ �����´�.
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
	// �ӽ÷� PŰ�� ������.
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
				if (LastWidget.IsEmpty()) // �������� �� ������ٸ�
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