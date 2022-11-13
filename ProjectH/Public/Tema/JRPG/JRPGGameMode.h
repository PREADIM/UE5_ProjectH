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
struct FLiveUnit
{
	GENERATED_USTRUCT_BODY()
public:
	FLiveUnit();
	FLiveUnit(class AJRPGUnit* U, bool b);

	class AJRPGUnit* Unit;
	bool bLive;

};


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





UCLASS()
class PROJECTH_API AJRPGGameMode : public AGameModeBase
{
	GENERATED_BODY()


// 배틀 시작 -> 캐릭터가 공격을 했거나, 받았을시에 JRPG상태로 들어가기 위한 함수.
// ㄴ 리스트 초기화 -> 모든 유닛에 대한 순서 초기화. (배틀 시작시에 딱 한번 사용)
// ㄴ 턴 시작 -> 리스트의 가장 위에있는 유닛의 턴 시작.
// ㄴ 컴포넌트 배틀시작 -> 각 유닛이 가지고 있는 컴포넌트의 배틀 시작 함수 실행.

// 턴 종료 -> 게임 모드의 턴 종료는 컴포넌트에서 호출한다. 리스트 맨위를 맨 아래로 넣는다.
// ㄴ 다시 턴 시작 함수를 호출한다. 

// 예외상황
// ㄴ 해당 순서의 유닛이 죽어있는 경우 -> 해당 캐릭터로 리스트를 for range로 찾아서 삭제. 위젯에서도 삭제
// ㄴ 해당 순서의 유닛이 상태이상인 경우 -> 턴시작 함수에서 공격가능한 상태인지 판별한다.
// ㄴ 적이 다 죽었거나, 아군이 다 죽었을 경우 -> 유닛의 공격 당한 함수에서 팀이 다죽었는지 판별. 그리고 끝내기.
public:
	AJRPGGameMode();
	void SetDataTable(UDataTable*& Table, FString TablePath); // 데이터 테이블 적용 함수.
	virtual void PostLogin(APlayerController* Login) override;



public:
	class AJRPGUnit* GetCharacterSpawn(int32 CharacterNum, FTransform UnitLocation);
	class AJRPGUnit* GetEnermySpawn(int32 CharacterNum, FTransform UnitLocation);
	bool GetBattleField(int32 FieldNum);
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
	//UPROPERTY(BlueprintReadWrite)
		//TArray<FLiveUnit> OwnerList; // 그냥 캐릭터의 리스트 죽으면 체크
	UPROPERTY(BlueprintReadWrite)
		TArray<class AJRPGUnit*> OwnerList;
	// 아군의 실질적인 살아있는 수.


	//캐릭터 스텟을 레벨로 검색해서 가져온다.
	FJRPGCharStat GetCharStat(int32 CharNum, int32 Level);
	
	void SetCurrentExpAndNextExp(); // 가지고있는 유닛의 현재 경험치와, 다음렙업에 필요한 경험치를 가져온다.




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



	UPROPERTY()
		class ABattleField* CurrentField;
	FVector FieldLocation;
	// 배틀필드 클래스를 가져오기. 배틀필드 또한 지역 넘버를 가지고있고 해당 위치를 데이터베이스에서 가져옴.



	/*------------------
	
		Save & Load
	
	------------------*/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UJRPGSave* JRPGSave; // 세이브


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
