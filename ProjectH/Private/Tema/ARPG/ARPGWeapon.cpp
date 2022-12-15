// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/ARPGWeapon.h"

// Sets default values
AARPGWeapon::AARPGWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SwordMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwordMesh"));

	SwordCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("SwordCollision"));
	SwordCollision->SetupAttachment(SwordMesh);

	SwordCollision->OnComponentBeginOverlap.AddDynamic(this, &AARPGWeapon::SwordBeginOverlap);

}

// Called when the game starts or when spawned
void AARPGWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AARPGWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AARPGWeapon::SwordBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != GetOwner() && OtherActor->GetOwner() != GetOwner())
	{

	}
}
