// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/Maze/EnermySpawnActor.h"
#include "Tema/ARPG/ARPGEnermy.h"
#include "Tema/ARPG/ARPGUnit.h"

// Sets default values
AEnermySpawnActor::AEnermySpawnActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Collision->OnComponentBeginOverlap.AddDynamic(this, &AEnermySpawnActor::OverlapUnit);
	Collision->OnComponentEndOverlap.AddDynamic(this, &AEnermySpawnActor::OverlapEndUnit);
}

// Called when the game starts or when spawned
void AEnermySpawnActor::BeginPlay()
{
	Super::BeginPlay();
	DefaultLocation = GetActorLocation();
}

void AEnermySpawnActor::OverlapUnit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AARPGUnit* Unit = Cast<AARPGUnit>(OtherActor);
	if (Unit)
	{
		if (!SpawnEnermy) // ó�� ����
		{
			SpawnRandEnermy();
			if (SpawnEnermy)
			{
				AttachToActor(SpawnEnermy, FAttachmentTransformRules::SnapToTargetIncludingScale);
				_DEBUG("Attach Spawner");
			}
		}
		else
		{
			SpawnEnermy->SetActorHiddenInGame(true);
			_DEBUG("Hidden false");
		}

	}

}


void AEnermySpawnActor::OverlapEndUnit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (SpawnEnermy)
	{
		SpawnEnermy->SetActorLocation(DefaultLocation);
		SpawnEnermy->SetActorHiddenInGame(true);
	}

}


void AEnermySpawnActor::SpawnRandEnermy()
{
	int32 Index = FMath::RandRange(0, BP_Enermys.Num());
	FActorSpawnParameters Param;
	Param.Owner = this;
	Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	SpawnEnermy = GetWorld()->SpawnActor<AARPGEnermy>(BP_Enermys[Index], GetActorLocation(), GetActorRotation(), Param);
}
