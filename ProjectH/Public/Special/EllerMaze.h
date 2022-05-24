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

};
