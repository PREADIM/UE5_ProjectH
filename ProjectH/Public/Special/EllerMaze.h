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

	FName EastSocket = FName("Y"); // ��
	FName SouthSocket = FName("-X"); // ��


	UPROPERTY(EditAnywhere)
		TArray<TSubclassOf<class AARPGEnermy>> BP_Enermys;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AEnermySpawnActor> BP_EnermySapwn; // ���ʹ� ������ ����. �ش� ���Ϳ� Overlap�Ǹ� ���� ��ȯ�ϴ� ������� �Ѵ�.

	UPROPERTY()
		TArray<FIndexAndRotation> SpawnIndexs; // ������ �ε����� �־�ΰ� �������� ����.

	UPROPERTY()
		TArray<class AEnermySpawnActor*> EnermySpawner;

public:
	void CreateMaze(); // ȣ���ϸ� �̷� ����.

	/*-----------------
		Eller Maze
	------------------*/
	bool Union(int32 A, int32 B); // �׷� ��ġ��
	int32 FindGroup(int32 num); // �׷�ã��.
	void Init(); //  �ʱ�ȭ
	void Start(); // �̷� ����� ����.
	void CreateRandomMaze(int32 CurrentCol); // �� �࿡�� �������� �׷� ��ġ��
	void NextColumn(int32 CurrentCol); // ���� ���� ���������� ���� ������ �Ǵ��ϱ�.
	void LastLine(int32 LastRow); // ������ ���� �״�� �����;��Ѵ�.


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
	MazeDirection RetMazeDir(OpenWallDir OWD); // OpenWallDir�� Ȯ���ؼ� �ո� ��� �������� �ٶ󺸰� �ϴ� ��.

};
