// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "GameFramework/PlayerController.h"
#include "Tema/JRPG/JRPGCharStat.h"
#include "JRPGPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API AJRPGPlayerController : public APlayerController
{
	GENERATED_BODY()


// 전투가 시작되면 특정 맵으로 이동해서 해당 구역에서 전투를 하게한다. 즉, 돌아다니던 원본 데이터를 지우지않고, 
// 그냥 전투 맵에서 캐릭터를 생성해서 쓰는 것이다.


public:
	virtual void BeginPlay();
	virtual void OnPossess(APawn* NewPawn) override;


public:
	// 메인 위젯이 두개이다. 평상시 위젯과, JRPG때의 위젯. 서로 다른 위젯이고 배틀일때 달라진다.

	void CameraPossess(FVector Location);
	void CameraSetUp(FVector Location);
	void ExitCamera();
	void PlayBattleMode(TArray<int32> EnermyUnits);

	void GameEndSpawnCharacter();





public:
	UPROPERTY(VisibleAnywhere)
		class AJRPGGameMode* GM;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AJRPGCamera> BP_Camera;
	class AJRPGCamera* DynamicCamera;


	UPROPERTY()
		FTransform FieldLocation;
	UPROPERTY()
		int32 RepreCharacterNum; // 대표캐릭터
	UPROPERTY()
		class AJRPGUnit* RepreCharacter;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<int32> CurrentParty; // 현재 선택되어있는 파티리스트
	UPROPERTY(VisibleAnywhere)
		int32 CurrentFieldNum; // 필드 정보


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<int32> HaveCharList; // 가지고 있는 전체 캐릭터 넘버 (추후 세이브 로드 해야함).
	// 나중에 퀘스트 넘버처럼 번호와 BP 패스를 가지고 해당 넘버만 저장하고 불러오는 형식으로 만들어도될듯.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TMap<int32, FJRPGCharStat> HaveCharStat; // 가지고 있는 캐릭터의 스텟들.
};
