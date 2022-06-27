// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/JRPGPlayerController.h"
#include "Tema/JRPG/JRPGCamera.h"
#include "Tema/JRPG/JRPGGameMode.h"




void AJRPGPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// ���⼭ ��ǥĳ���� ��ȣ�� ���⼭ �����ؼ� �����Ͽ� �����Ѵ�.
}

void AJRPGPlayerController::OnPossess(APawn* NewPawn)
{
	Super::OnPossess(NewPawn);
	RepreCharacter = Cast<AJRPGUnit>(NewPawn);
	GM->SetSaveJRPG();
}


void AJRPGPlayerController::CameraPossess(FVector Location)
{
	if (BP_Camera)
	{
		DynamicCamera = GetWorld()->SpawnActor<AJRPGCamera>(BP_Camera, FTransform(Location));
		UnPossess();
		OnPossess(DynamicCamera);
		// ���̳��� ī�޶� ���ǵȰŸ� ��Ʋ ������ �Ǿ��ٴ� ��.
	}
}


void AJRPGPlayerController::CameraSetUp(FVector Location)
{
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
		}
		GM->BattleStart(CurrentFieldNum, EnermyUnits);	

	}
}


void AJRPGPlayerController::GameEndSpawnCharacter()
{
	
}