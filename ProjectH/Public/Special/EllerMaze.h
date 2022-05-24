// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#define MAZE_COUNT 30
#include "ProjectH.h"
#include <iostream>
#include "GameFramework/Actor.h"
#include "EllerMaze.generated.h"

using namespace std;



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
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<class AMazeBase*> MazeActors;

	UPROPERTY()
		TArray<int32> UnionList;

	Maze List[MAZE_COUNT][MAZE_COUNT];

	FName EastSocket = FName("Y");
	FName SouthSocket = FName("-X");

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

};
