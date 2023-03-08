// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "GameFramework/GameModeBase.h"
#include "Tema/JRPG/JRPGCharStat.h"
#include "Tema/JRPG/JRPGDropStruct.h"
#include "JRPGGameMode.generated.h"

/**
 * 
 */

struct FEnermys;

USTRUCT(BlueprintType)
struct FPriorityUnit
{
	GENERATED_USTRUCT_BODY()

public:
	FPriorityUnit();
	FPriorityUnit(class AJRPGUnit* Char);

	UPROPERTY(BlueprintReadWrite)
		class AJRPGUnit* Unit;
	UPROPERTY(BlueprintReadWrite)
		int32 Priority;
};


struct PriorityUnitFunc
{
	bool operator()(const FPriorityUnit& A, const FPriorityUnit& Other) const
	{
		return A.Priority > Other.Priority; // 최대힙
	}
};


USTRUCT(BlueprintType)
struct FBuffIconStruct : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString IconName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UTexture2D* IconTexture;
};



UCLASS()
class PROJECTH_API AJRPGGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AJRPGGameMode();
	virtual void PostLogin(APlayerController* Login) override;

public:


	class AJRPGUnit* GetCharacterSpawn(int32 CharacterNum, FTransform UnitLocation);
	class AJRPGUnit* GetEnermySpawn(int32 CharacterNum, FTransform UnitLocation);
	bool GetBattleField(int32 FieldNum);
	FBuffIconStruct* GetBuffIcon(FString IconName);


	/*------------------------------
		JRPG 게임 흐름 함수들
	-------------------------------*/

	void BattleStart(int32 FieldNum, TArray<FEnermys> Enermys);
	void TurnStart();

	UFUNCTION(BlueprintCallable)
		void TurnEnd();

	void TurnListInit();
	void SetUnitListArray(); // 힙 정렬로 우선순위 정렬 한 것을 큐로 저장.
	void TurnListSet();
	void GameEnd(bool bWinner); // 아군이나 적이 다 죽었을 경우 해당 게임을 끝내기.
	// true는 플레이어가 승자, false는 적이 승리.


	void ReturnWorld(); // 게임끝나고 원래 전장으로 돌아가기

	void SetOwnerUnits(); // 아군 유닛 스폰 및 리스트 할당.
	void SetEnermyUnits(TArray<FEnermys> Enermys); // 적군 유닛 스폰 및 리스트 할당.

	void SetControllerInit();


public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class AJRPGPlayerController* OwnerController;
	UPROPERTY()
		TArray<FPriorityUnit> UnitList; // 우선순위 힙정렬을 위한 배열
	UPROPERTY()
		TArray <FPriorityUnit> SetUnitList; // 정렬된 것을 여기에 차례차례 정렬해서 사용

	UPROPERTY(BlueprintReadWrite)
		TArray<FPriorityUnit> EnermyUnits;
	UPROPERTY(BlueprintReadWrite)
		TArray<class AJRPGUnit*> EnermyList;
	// 적의 살아있는 수. UI로도 연동

	UPROPERTY(BlueprintReadWrite)
		TArray<FPriorityUnit> OwnerUnits;
	UPROPERTY(BlueprintReadWrite)
		TArray<class AJRPGUnit*> OwnerList; // 아군의 실질적인 살아있는 수.
	


	//----------------------------------------------------------

	FJRPGCharStat GetCharStat(int32 CharNum, int32 Level); //캐릭터 스텟을 레벨로 검색해서 가져온다.

	void SetControllerInCharacterStat(); // 가지고있는 유닛의 스탯을 가져온다.

	//----------------------------------------------------------


	/* 테마들은 각각 게임모드를 게임 인스턴스처럼 사용해야함. 세이브 파일도 물론 여기에 있다. */
	UPROPERTY(VisibleAnywhere)
		class UDataTable* FieldTable; // 필드 데이터 테이블
	UPROPERTY(VisibleAnywhere)
		class UDataTable* CharListTable; // JPRG캐릭터 테이블.
	UPROPERTY(VisibleAnywhere)
		class UDataTable* EnermyListTable; // JPRG캐릭터 테이블.
	UPROPERTY(VisibleAnywhere)
		class UDataTable* CharStatTablePaths; // 캐릭터 스탯이 있는 테이블들의 경로들.
	UPROPERTY(VisibleAnywhere)
		class UDataTable* FieldEnermyDropTable; // 필드 적의 떨구는 경험치 및 아이템.
	UPROPERTY(VisibleAnywhere)
		class UDataTable* IconTextureTable; // 각종 버프 아이콘 테이블



	UPROPERTY(BlueprintReadOnly)
		class ABattleField* CurrentField;
	FVector FieldLocation;
	// 배틀필드 클래스를 가져오기. 배틀필드 또한 지역 넘버를 가지고있고 해당 위치를 데이터베이스에서 가져옴.


	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget> BP_BattleTutorialWidget;
	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget> BP_PartyTutorialWidget;


	void BattleTutorialStart();
	void PartyTutorialStart();

	/*------------------
	
		Save & Load
	
	------------------*/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UJRPGSave* JRPGSave; // 세이브

	bool bBattleTutorial = false; // 튜토리얼을 했는지 안했는지 알아오는 것.
	bool bPartyTutorial = false; // 튜토리얼을 했는지 안했는지 알아오는 것.

	UPROPERTY(EditAnywhere)
		int32 MaxCharCnt; // 이 게임이 가지는 최대 캐릭터 수
	// 아직은 활용 x 몇개중에 몇개 보유 확인할 때 필요할 듯.

	void SetSaveJRPG();

	void SetSaveEnermyUnits(class AJRPGEnermy* FieldEnermy);
	bool GetSaveEnermyUnits(int32 EnermyUnitNum); 
	// 이 둘은 필드의 적이 죽엇는지 살았는지 세이브하는 함수들.

	FJRPGDropStruct GetFieldEnermyDropStruct(int32 EnermyUnitNum);
	// 필드 적이 죽을 때 남길 아이템.

	UPROPERTY()
		class AJRPGEnermy* CurrentBattleEnermy; // 현재 싸우는 필드 유닛을 저장.
	// 게임이 끝나고 죽었는지 살았는지 알아야 하기때문.
	
	UPROPERTY(BlueprintReadOnly)
		int32 KillCnt; // 추후 기믹을 위해. 카운트
};
