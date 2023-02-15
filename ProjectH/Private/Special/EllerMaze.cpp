// Fill out your copyright notice in the Description page of Project Settings.


#include "Special/EllerMaze.h"
#include "Special/MazeBase.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Tema/ARPG/Maze/EnermySpawnActor.h"

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
	CreateMaze();
}

void AEllerMaze::CreateMaze()
{
	if (!BP_MazeActor)
		return;

	if (!UnionList.IsEmpty())
		UnionList.Empty();

	UnionList.Init(0, MAZE_COUNT * MAZE_COUNT);
	Init();
	Start();
	MazeActors.Emplace(GetWorld()->SpawnActor<AMazeBase>(BP_MazeActor, GetActorLocation(), FRotator::ZeroRotator));
	if (MazeActors.IsValidIndex(0))
		MazeActors[0]->NorthInvisible();
	SpawnMaze();

	SpawnIndexs.Empty(); // ���� �� ������ ��ġ�迭 �ʱ�ȭ.
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
		RandomEnermyGroup(i); // ���ʹ� ���� �׷�
	}

	LastLine(MAZE_COUNT - 1);
}


void AEllerMaze::CreateRandomMaze(int32 CurrentCol)
{
	for (int32 i = 0; i < MAZE_COUNT; i++)
	{
		switch (FMath::RandRange(1, 3))
		{
		case 1: // ���������� �պ��ϱ�.
			if (i + 1 == MAZE_COUNT)
				break;

			if (Union(List[CurrentCol][i].Group, List[CurrentCol][i + 1].Group))
			{
				List[CurrentCol][i].OpenDir.East = true;
				List[CurrentCol][i + 1].OpenDir.West = true;

				if (i - 1 >= 0)
				{
					// ���� �׷��̸� ���ʿ� �� �ʿ������, ���� �׷��� �ƴѰ�쿡 �ձ�.
					if (FindGroup(List[CurrentCol][i].Group) != FindGroup(List[CurrentCol][i - 1].Group))
					{
						if (List[CurrentCol][i - 1].OpenDir.East == false) // ���� ���� �����ִ� ���
							List[CurrentCol][i - 1].OpenDir.East = true; // �������� ���͸� �ּ�ȭ �ϱ�����.
					}
				}
			}
			break;

		case 2: // �������� �պ��ϱ�.
			if (i - 1 < 0)
				break;
			
			if (Union(List[CurrentCol][i].Group, List[CurrentCol][i - 1].Group))
			{
				List[CurrentCol][i].OpenDir.West = true;
				List[CurrentCol][i - 1].OpenDir.East = true;
			}
			break;

		default: // �պ� ���ϱ�
			if (i - 1 >= 0)
			{
				// ���� �׷��̸� ���ʿ� �� �ʿ������, ���� �׷��� �ƴѰ�쿡 �ձ�.
				if (FindGroup(List[CurrentCol][i].Group) != FindGroup(List[CurrentCol][i - 1].Group))
				{
					if (List[CurrentCol][i - 1].OpenDir.East == false) // ���� ���� �����ִ� ���
						List[CurrentCol][i - 1].OpenDir.East = true; // �������� ���͸� �ּ�ȭ �ϱ�����.
				}
			}
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
			// �׷� �߰�.
			GroupMap.Add(Group);
			GroupMap[Group].Add(i);
		}
		else
		{
			GroupMap[Group].Add(i);
		}
	}

	for (auto it = GroupMap.CreateConstIterator(); it; ++it) // TMap ��ü ����
	{
		bool flag = false; // ���� �˰����� �׷쳻���� �� �Ѱ��� ���� ������ �����߰����Ѵ�.
		for (int32 i = 0; i < it->Value.Num(); i++)
		{
			int32 Row = it->Value[i];
			if (FMath::RandRange(0, 1) == 1)
			{			
				if (Union(List[CurrentCol][Row].Group, List[CurrentCol + 1][Row].Group))
				{
					List[CurrentCol][Row].OpenDir.South = true;
					List[CurrentCol + 1][Row].OpenDir.North = true;
					flag = true;
				}
			}
			else
			{
				List[CurrentCol][Row].OpenDir.South = true; // �������� ���͸� ���ֱ� ����.
			}
		}

		if (!flag) // �Ѱ��� �׷쳻���� ���� ������ �ո��� �ʾ��� ���.
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

/*�̷� ������*/
void AEllerMaze::SpawnMaze()
{
	if (MazeActors.IsEmpty())
		return;

	int ColCnt = 0;
	for (int32 x = 0; x < MAZE_COUNT; x++)
	{
		ColCnt = x * MAZE_COUNT;
		for (int32 y = 0; y < MAZE_COUNT - 1; y++)
		{
			if (!MazeActors.IsValidIndex(y + ColCnt))
			{
				//_DEBUG("NULL MAZE");
				return;
			}

			FVector Location = MazeActors[y + ColCnt]->Floor->GetSocketLocation(EastSocket);
			MazeActors.Emplace(GetWorld()->SpawnActor<AMazeBase>(BP_MazeActor, Location, FRotator::ZeroRotator));
		}

		if (x != MAZE_COUNT - 1)
		{
			FVector Location = MazeActors[ColCnt]->Floor->GetSocketLocation(SouthSocket);
			MazeActors.Emplace(GetWorld()->SpawnActor<AMazeBase>(BP_MazeActor, Location, FRotator::ZeroRotator));
		}
	}

	MazeOpenWall();
	SpawnEnermy(); // ���ʹ� ���� ���͸� ��ȯ�Ѵ�.
}

/* �̷� �� ���� */
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

void AEllerMaze::RandomEnermyGroup(int32 Colum)
{
	int32 Index = FMath::RandRange(0, MAZE_COUNT - 1);
	MazeDirection MD = RetMazeDir(List[Colum][Index].OpenDir); // ���� ��������.

	SpawnIndexs.Emplace(FIndexAndRotation(Colum * MAZE_COUNT + Index, MD)); // �׷� ����.
}


void AEllerMaze::SpawnEnermy()
{
	FActorSpawnParameters Param;
	Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	for (FIndexAndRotation IR : SpawnIndexs)
	{
		FVector Location = MazeActors[IR.Index]->GetActorLocation();
		FRotator Rot;

		switch (IR.Direction)
		{
		case MazeDirection::North :
			Rot = FRotator(0.f, 0.f, 0.f);
			break;
		case MazeDirection::South :
			Rot = FRotator(0.f, 180.f, 0.f);
			break;
		case MazeDirection::West :
			Rot = FRotator(0.f, -90.f, 0.f);
			break;
		case MazeDirection::East :
			Rot = FRotator(0.f, 90.f, 0.f);
			break;
		default:
			Rot = FRotator(0.f, 0.f, 0.f);
			break;
		}
	
		GetWorld()->SpawnActor<AEnermySpawnActor>(BP_EnermySapwn, MazeActors[IR.Index]->GetActorLocation() + FVector(0.f, 0.f, 200.f), Rot, Param);
	}
}

MazeDirection AEllerMaze::RetMazeDir(OpenWallDir OWD)
{
	// �̷δ� ������ �Ʒ��� ���پ� �����Ǳ� ������ ������ �����ִٴ°� ������ �ٶ���ߵ�.
	if (OWD.North == true)
		return MazeDirection::North;

	// ���� �����ְ� �Ʒ��� �շ��ִ� ���� ĳ���Ͱ� ���̳� �Ʒ����� �ü��ִٴ� �Ҹ�
	if (OWD.South == true)
		return MazeDirection::South;

	if (OWD.West || OWD.East)
		return MazeDirection::West;


	return MazeDirection::East;

	/*TArray<int32> DirNums;

	if (OWD.North == true)
		DirNums.Add(4);

	if (OWD.South == true)
		DirNums.Add(3);

	if (OWD.West == true)
		DirNums.Add(2);

	if (OWD.East == true)
		DirNums.Add(1);

	int32 Index = FMath::RandRange(0, DirNums.Num() - 1);
	switch (Index)
	{
	case 1:
		return MazeDirection::East;
	case 2:
		return MazeDirection::West;
	case 3:
		return MazeDirection::South;
	case 4:
		return MazeDirection::North;
	default:
		return MazeDirection::South;
	}*/
}
