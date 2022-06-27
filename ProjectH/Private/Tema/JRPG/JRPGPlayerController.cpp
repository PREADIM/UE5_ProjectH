// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/JRPGPlayerController.h"
#include "Tema/JRPG/JRPGCamera.h"
#include "Tema/JRPG/JRPGGameMode.h"




void AJRPGPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 여기서 대표캐릭터 번호를 여기서 생성해서 스폰하여 빙의한다.
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
		// 다이나믹 카메라가 빙의된거면 배틀 시작이 되었다는 뜻.
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
		}
		GM->BattleStart(CurrentFieldNum, EnermyUnits);	

	}
}


void AJRPGPlayerController::GameEndSpawnCharacter()
{
	
}