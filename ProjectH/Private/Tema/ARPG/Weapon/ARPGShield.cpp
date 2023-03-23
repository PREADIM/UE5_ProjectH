// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/Weapon/ARPGShield.h"

// Sets default values
AARPGShield::AARPGShield()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ShieldMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ShieldMesh"));

	ShieldCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("ShieldCollision"));
	ShieldCollision->SetupAttachment(ShieldMesh);


	BlockingDEF = 1.f;
}

// Called when the game starts or when spawned
void AARPGShield::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AARPGShield::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AARPGShield::SetWeaponCollision(bool bFlag)
{
	if (bFlag)
	{
		ShieldCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);	
	}
	else
	{
		ShieldCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);	
	}
}

void AARPGShield::SetPhysics()
{
	Super::SetPhysics();
	ShieldMesh->SetSimulatePhysics(true);
	ShieldMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AARPGShield::SetOwnerNoSee(bool bFlag)
{
	if (bFlag)
	{
		ShieldMesh->SetOnlyOwnerSee(false);
		ShieldMesh->SetOwnerNoSee(true);
		ShieldMesh->SetCastShadow(true);
		ShieldMesh->SetCastHiddenShadow(true);
	}
	else
	{
		ShieldMesh->SetOnlyOwnerSee(true);
		ShieldMesh->SetOwnerNoSee(false);
		ShieldMesh->SetCastShadow(false);
		SetWeaponCollision(false);
	}
	
}


