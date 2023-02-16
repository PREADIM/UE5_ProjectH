// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#define MAZE_COUNT 15
#include "ProjectH.h"
#include <iostream>
#include "GameFramework/Actor.h"
#include "EllerMaze.generated.h"

using namespace std;


enum class MazeDirection
{
	North,
	South,
	West,
	East
};


USTRUCT()
struct FIndexAndRotation
{
	GENERATED_USTRUCT_BODY()

public:
	int32 Index;
	MazeDirection Direction;

	FIndexAndRotation()
	{

	}

	FIndexAndRotation(int32 Num, MazeDirection MD)
	{
		Index = Num;
		Direction = MD;
	}
};


struct OpenWallDir
{
public:
	bool North;
	bool South;
	bool East;
	bool West;

	OpenWallDir()
	{
		North = false;
		South = false;
		East = false;
		West = false;
	}
};



struct Maze
{
public:
	int32 Group;
	OpenWallDir OpenDir;

	Maze()
	{
		Group = 0;
		OpenDir = OpenWallDir();
	}
};

UCLASS()
class PROJECTH_API AEllerMaze : public AActor
{
	GENERATED_BODY()

	
public:	
	// Sets default values for this actor's properties
	AEllerMaze();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditANywhere)
		TSubclassOf<class AMazeBase> BP_MazeActor;

	UPROPERTY(VisibleAnywhere)
		TArray<class AMazeBase*> MazeActors;

	UPROPERTY()
		TArray<int32> UnionList;

	Maze List[MAZE_COUNT][MAZE_COUNT];

	FName EastSocket = FName("Y"); // 동
	FName SouthSocket = FName("-X"); // 남


	UPROPERTY(EditAnywhere)
		TArray<TSubclassOf<class AARPGEnermy>> BP_Enermys;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AEnermySpawnActor> BP_EnermySapwn; // 에너미 스폰할 액터. 해당 액터에 Overlap되면 적을 소환하는 방식으로 한다.

	UPROPERTY()
		TArray<FIndexAndRotation> SpawnIndexs; // 스폰할 인덱스만 넣어두고 마지막에 스폰.

	UPROPERTY()
		TArray<class AEnermySpawnActor*> EnermySpawner;

public:
	void CreateMaze(); // 호출하면 미로 생성.

	/*-----------------
		Eller Maze
	------------------*/
	bool Union(int32 A, int32 B); // 그룹 합치기
	int32 FindGroup(int32 num); // 그룹찾기.
	void Init(); //  초기화
	void Start(); // 미로 만들기 시작.
	void CreateRandomMaze(int32 CurrentCol); // 그 행에서 랜덤으로 그룹 합치기
	void NextColumn(int32 CurrentCol); // 현재 열을 다음행으로 내릴 것인지 판단하기.
	void LastLine(int32 LastRow); // 마지막 열은 그대로 내려와야한다.


	/*---------------
		Create Maze
	----------------*/
	void SpawnMaze();
	void MazeOpenWall();

	/*------------------
		Ramdom Enermy
	--------------------*/
	void RandomEnermyGroup(int32 Colum);
	void SpawnEnermy();
	MazeDirection RetMazeDir(OpenWallDir OWD); // OpenWallDir을 확인해서 뚫린 방면 랜덤으로 바라보게 하는 것.

};
