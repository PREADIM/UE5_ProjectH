// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "GameFramework/GameModeBase.h"
#include "Tema/JRPG/JRPGUnit.h"
#include "JRPGGameMode.generated.h"

/**
 * 
 */
USTRUCT()
struct FPriorityUnit
{
	GENERATED_USTRUCT_BODY()

public:
	FPriorityUnit();
	FPriorityUnit(class AJRPGUnit* Char);

	class AJRPGUnit* Unit;
	int32 Priority;
};


struct PriorityUnitFunc
{
	bool operator() (const FPriorityUnit& A, const FPriorityUnit& Other) const
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
	virtual void PostLogin(APlayerController* Login) override;



public:
	AJRPGUnit* GetCharacterSpawn(int32 CharacterNum, FTransform UnitLocation);
	AJRPGUnit* GetEnermySpawn(int32 CharacterNum, FTransform UnitLocation);
	bool GetBattleField(int32 FieldNum);
	void BattleStart(int32 FieldNum, TArray<int32> Enermys);
	void TurnStart();
	void TurnEnd();

	void TurnListInit();
	void TurnListSet();

	void GameEnd(); // 아군이나 적이 다 죽었을 경우 해당 게임을 끝내기.
	// 적이나 아군이나 어처피 AJRPGUnit이므로 TakeDamage에서 데미지를 주고, 캐스트 하면 될듯하다.

	void ReturnWorld(); // 게임끝나고 원래 전장으로 돌아가기

	void SetOwnerUnits(); // 아군 유닛 스폰 및 리스트 할당.
	void SetEnermyUnits(TArray<int32> Enermys); // 적군 유닛 스폰 및 리스트 할당.

	void FirstStartSaveStat(); // 마땅히 저장되어있는 세이브파일이 없다는 것은 가장 기초 캐릭터가 없다는 뜻이므로.
	// 기초 캐릭터를 저장함.

	void SetControllerInit();
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class AJRPGPlayerController* OwnerController;
	UPROPERTY()
		TArray<FPriorityUnit> UnitList; 

	UPROPERTY()
		TArray<class AJRPGUnit*> EnermyUnits;
	UPROPERTY()
		TArray<class AJRPGUnit*> OwnerUnits;
	// 적이나 캐릭터가 공격받아 배틀 start를 하기전에 먼저 이 게임모드에 적절하게 먼저 넣게 한다.
	// 결국엔 이것은 캐릭터안에 있는 것을 넣는것. 
	// 만약 원신처럼 대표캐릭터를 바꿀수 있는 게임이라면, 캐릭터를 스폰하면서 보유 캐릭터도 보낸다.



	/* 테마들은 각각 게임모드를 게임 인스턴스처럼 사용해야함. 세이브 파일도 물론 여기에 있다. */
	UPROPERTY(VisibleAnywhere)
		class UDataTable* FieldTable; // 필드 데이터 테이블
	UPROPERTY(VisibleAnywhere)
		class UDataTable* CharListTable; // JPRG캐릭터 테이블.
	UPROPERTY(VisibleAnywhere)
		class UDataTable* EnermyListTable; // JPRG캐릭터 테이블.


	UPROPERTY()
		class ABattleField* CurrentField;
	FVector FieldLocation;
	// 배틀필드 클래스를 가져오기. 배틀필드 또한 지역 넘버를 가지고있고 해당 위치를 데이터베이스에서 가져옴.



	/*------------------
	
		Save & Load
	
	------------------*/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UJRPGSave* JRPGSave; // 세이브

	void SetSaveJRPG();
	
};
