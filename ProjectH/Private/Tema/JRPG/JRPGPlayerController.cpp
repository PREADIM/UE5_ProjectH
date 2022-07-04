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

	// ★★ 원래는 이미 있는 RepreCharacter로 HaveCharStat에서 받아오는 것이지만, 나중에 구현.

	GM->SetControllerInit(); // OnPossess를 하면 왜인지는 모르겠으나, 값이 초기화된다. 그래서 다시 설정.
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
		}
		GM->BattleStart(CurrentFieldNum, EnermyUnits);	

	}
}


void AJRPGPlayerController::GameEndSpawnCharacter()
{
	
}