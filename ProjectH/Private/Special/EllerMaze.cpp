// Fill out your copyright notice in the Description page of Project Settings.


#include "Special/EllerMaze.h"
#include "Special/MazeBase.h"

// Sets default values
AEllerMaze::AEllerMaze()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AEllerMaze::BeginPlay()
{
	Super::BeginPlay();
	UnionList.Init(0, MAZE_COUNT * MAZE_COUNT);
	CreateMaze();
}

void AEllerMaze::CreateMaze()
{
	Init();
	Start();
	MazeActors.Add(GetWorld()->SpawnActor<AMazeBase>(GetActorLocation(), FRotator::ZeroRotator));
	if (MazeActors.IsValidIndex(0))
		MazeActors[0]->NorthInvisible();
	SpawnMaze();

}


bool AEllerMaze::Union(int32 A, int32 B)
{
	int32 GroupA = FindGroup(A);
	int32 GroupB = FindGroup(B);

	if (GroupA == GroupB)
		return false;


	if (GroupA < GroupB)
	{
		UnionList[GroupB] = GroupA;
		return true;
	}
	else
	{
		UnionList[GroupA] = GroupB;
		return true;
	}


}


int32 AEllerMaze::FindGroup(int32 num)
{
	if (UnionList[num] == num)
		return num;
	else
		return UnionList[num] = FindGroup(UnionList[num]);
	
}


void AEllerMaze::Init()
{
	int32 InitNum = 0;
	for (int32 i = 0; i < UnionList.Num(); i++)
		UnionList[i] = i;


	int32 GroupNumber = 0;
	for (int32 i = 0; i < MAZE_COUNT; i++)
	{
		for (int32 y = 0; y < MAZE_COUNT; y++)
		{
			List[i][y].Group = GroupNumber++;
		}
	}


}


void AEllerMaze::Start()
{
	for (int32 i = 0; i < MAZE_COUNT - 1; i++)
	{
		CreateRandomMaze(i);
		NextColumn(i);
	}

	LastLine(MAZE_COUNT - 1);
}


void AEllerMaze::CreateRandomMaze(int32 CurrentCol)
{
	for (int32 i = 0; i < MAZE_COUNT; i++)
	{
		switch (FMath::RandRange(1, 3))
		{
		case 1: // 오른쪽으로 합병하기.
			if (i + 1 == MAZE_COUNT)
				break;

			if (Union(List[CurrentCol][i].Group, List[CurrentCol][i + 1].Group))
			{
				List[CurrentCol][i].OpenDir.East = true;
				List[CurrentCol][i + 1].OpenDir.West = true;
			}
			break;

		case 2: // 왼쪽으로 합병하기.
			if (i - 1 < 0)
				break;

			if (Union(List[CurrentCol][i].Group, List[CurrentCol][i - 1].Group))
			{
				List[CurrentCol][i].OpenDir.West = true;
				List[CurrentCol][i - 1].OpenDir.East = true;
			}
			break;

		default: // 합병 안하기
			break;
		}
	}
}


void AEllerMaze::NextColumn(int32 CurrentCol)
{
	TMap<int32, TArray<int32>> GroupMap;

	for (int32 i = 0; i < MAZE_COUNT; i++)
	{
		int Group = FindGroup(List[CurrentCol][i].Group);
		if (GroupMap.Find(Group) == nullptr)
		{
			GroupMap.Add(Group);
			GroupMap[Group].Add(i);
		}
		else
		{
			GroupMap[Group].Add(i);
		}

	}

	for (auto it = GroupMap.CreateConstIterator(); it; ++it) // TMap 전체 돌기
	{
		bool flag = false; // 엘러 알고리즘은 그룹내에서 단 한개라도 다음 행으로 내려야가야한다.
		for (int32 i = 0; i < it->Value.Num(); i++)
		{
			if (FMath::RandRange(0, 1) == 1)
			{
				int32 Row = it->Value[i];
				if (Union(List[CurrentCol][Row].Group, List[CurrentCol + 1][Row].Group))
				{
					List[CurrentCol][Row].OpenDir.South = true;
					List[CurrentCol + 1][Row].OpenDir.North = true;
					flag = true;
				}
			}
		}

		if (!flag) // 한개도 그룹내에서 다음 행으로 뚫리지 않았을 경우.
		{
			int32 Random = FMath::RandRange(0, it->Value.Num() - 1);
			int32 RandomRow = it->Value[Random];

			if (Union(List[CurrentCol][RandomRow].Group, List[CurrentCol + 1][RandomRow].Group))
			{
				List[CurrentCol][RandomRow].OpenDir.South = true;
				List[CurrentCol + 1][RandomRow].OpenDir.North = true;
			}
		}
	}


}

void AEllerMaze::LastLine(int32 LastLineRow)
{
	for (int32 i = 0; i < MAZE_COUNT; i++)
	{
		if (i + 1 >= MAZE_COUNT)
			return;

		if (List[LastLineRow][i].Group != List[LastLineRow][i + 1].Group)
		{
			List[LastLineRow][i].OpenDir.East = true;
			List[LastLineRow][i + 1].OpenDir.West = true;
		}
	}
}

/*미로 생성기*/
void AEllerMaze::SpawnMaze()
{
	if (MazeActors.IsEmpty())
	{
		_DEBUG("MAZE NULL");
		return;
	}

	int ColCnt = 0;
	for (int32 x = 0; x < MAZE_COUNT; x++)
	{
		ColCnt = x * MAZE_COUNT;
		for (int32 y = 0; y < MAZE_COUNT - 1; y++)
		{
			if (!MazeActors.IsValidIndex(y + ColCnt))
			{
				_DEBUG("NULL MAZE");
				return;
			}

			FVector Location = MazeActors[y + ColCnt]->Floor->GetSocketLocation(EastSocket);
			MazeActors.Add(GetWorld()->SpawnActor<AMazeBase>(Location, FRotator::ZeroRotator));
		}

		if (x != MAZE_COUNT - 1)
		{
			FVector Location = MazeActors[ColCnt]->Floor->GetSocketLocation(SouthSocket);
			MazeActors.Add(GetWorld()->SpawnActor<AMazeBase>(Location, FRotator::ZeroRotator));
		}
	}

	MazeOpenWall();

}

/* 미로 문 열기 */
void AEllerMaze::MazeOpenWall()
{
	int CurCol = 0;
	for (int32 x = 0; x < MAZE_COUNT; x++)
	{
		CurCol = x * MAZE_COUNT;

		for (int32 y = 0; y < MAZE_COUNT; y++)
		{
			if (List[x][y].OpenDir.North)
				MazeActors[CurCol + y]->NorthInvisible();
	
			if (List[x][y].OpenDir.South)
				MazeActors[CurCol + y]->SouthInvisible();
			
			if (List[x][y].OpenDir.East)
				MazeActors[CurCol + y]->EastInvisible();
			
			if (List[x][y].OpenDir.West)
				MazeActors[CurCol + y]->WestInvisible();
		}
	}

	MazeActors[MAZE_COUNT - 1]->EastInvisible();

}
