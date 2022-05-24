// Fill out your copyright notice in the Description page of Project Settings.


#include "Special/MazeBase.h"

// Sets default values
AMazeBase::AMazeBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	NorthWall = CreateDefaultSubobject<class UStaticMeshComponent>(TEXT("NorthWall"));
	SouthWall = CreateDefaultSubobject<class UStaticMeshComponent>(TEXT("SouthWall"));
	WestWall = CreateDefaultSubobject<class UStaticMeshComponent>(TEXT("WestWall"));
	EastWall = CreateDefaultSubobject<class UStaticMeshComponent>(TEXT("EastWall"));
	Floor = CreateDefaultSubobject<class UStaticMeshComponent>(TEXT("Floor"));

	RootComponent = Floor;

	FString FloorPath = TEXT("StaticMesh'/Game/PROJECT/BP_CLASS/Asset/03_StaticMesh/TestFloor.TestFloor'");
	static ConstructorHelpers::FObjectFinder<class UStaticMesh> FloorMesh(*FloorPath);
	if (FloorMesh.Succeeded())
	{
		Floor->SetStaticMesh(FloorMesh.Object);
	}


	static FName FloorSocket[] = { {TEXT("XWall")}, {TEXT("-XWall")}, {TEXT("YWall")}, {TEXT("-YWall")} };
	for (FName Socket : FloorSocket)
	{
		if (!Floor->DoesSocketExist(Socket))
			return;
	}


	FString WallPath = TEXT("StaticMesh'/Game/PROJECT/BP_CLASS/Asset/03_StaticMesh/testGate.testGate'");
	static ConstructorHelpers::FObjectFinder<class UStaticMesh> WallMesh(*WallPath);
	if (WallMesh.Succeeded())
	{
		NorthWall->SetStaticMesh(WallMesh.Object);
		NorthWall->SetupAttachment(RootComponent, FloorSocket[0]);

		SouthWall->SetStaticMesh(WallMesh.Object);
		SouthWall->SetupAttachment(RootComponent, FloorSocket[1]);

		EastWall->SetStaticMesh(WallMesh.Object);
		EastWall->SetupAttachment(RootComponent, FloorSocket[2]);

		WestWall->SetStaticMesh(WallMesh.Object);
		WestWall->SetupAttachment(RootComponent, FloorSocket[3]);

	}

}

// Called when the game starts or when spawned
void AMazeBase::BeginPlay()
{
	Super::BeginPlay();
	
}


void AMazeBase::NorthInvisible()
{
	NorthWall->SetVisibility(false);
	NorthWall->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMazeBase::SouthInvisible()
{
	SouthWall->SetVisibility(false);
	SouthWall->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


void AMazeBase::WestInvisible()
{
	WestWall->SetVisibility(false);
	WestWall->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


void AMazeBase::EastInvisible()
{
	EastWall->SetVisibility(false);
	EastWall->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}