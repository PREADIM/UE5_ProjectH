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

	SpawnIndexs.Empty(); // 랜덤 적 스폰한 위치배열 초기화.
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
		RandomEnermyGroup(i); // 에너미 랜덤 그룹
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

				if (i - 1 >= 0)
				{
					// 같은 그룹이면 애초에 할 필요없으니, 같은 그룹이 아닌경우에 뚫기.
					if (FindGroup(List[CurrentCol][i].Group) != FindGroup(List[CurrentCol][i - 1].Group))
					{
						if (List[CurrentCol][i - 1].OpenDir.East == false) // 전에 벽이 막혀있는 경우
							List[CurrentCol][i - 1].OpenDir.East = true; // 겹쳐지는 액터를 최소화 하기위함.
					}
				}
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
			if (i - 1 >= 0)
			{
				// 같은 그룹이면 애초에 할 필요없으니, 같은 그룹이 아닌경우에 뚫기.
				if (FindGroup(List[CurrentCol][i].Group) != FindGroup(List[CurrentCol][i - 1].Group))
				{
					if (List[CurrentCol][i - 1].OpenDir.East == false) // 전에 벽이 막혀있는 경우
						List[CurrentCol][i - 1].OpenDir.East = true; // 겹쳐지는 액터를 최소화 하기위함.
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
			// 그룹 추가.
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
				List[CurrentCol][Row].OpenDir.South = true; // 겹쳐지는 액터를 없애기 위함.
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
	SpawnEnermy(); // 에너미 스폰 액터를 소환한다.
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

void AEllerMaze::RandomEnermyGroup(int32 Colum)
{
	int32 Index = FMath::RandRange(0, MAZE_COUNT - 1);
	MazeDirection MD = RetMazeDir(List[Colum][Index].OpenDir); // 방향 가져오기.

	SpawnIndexs.Emplace(FIndexAndRotation(Colum * MAZE_COUNT + Index, MD)); // 그룹 저장.
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
	// 미로는 위에서 아래로 한줄씩 생성되기 때문에 북쪽이 열려있다는건 북쪽을 바라봐야됨.
	if (OWD.North == true)
		return MazeDirection::North;

	// 위가 막혀있고 아래가 뚫려있는 경우는 캐릭터가 옆이나 아래에서 올수있다는 소리
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
