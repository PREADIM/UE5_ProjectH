// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/Shield/ARPGShield_Normal.h"

AARPGShield_Normal::AARPGShield_Normal()
{

	ShieldMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ShieldMesh"));

	ShieldCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("ShieldCollision"));
	ShieldCollision->SetupAttachment(ShieldMesh);

	ShieldCollision->OnComponentBeginOverlap.AddDynamic(this, &AARPGShield_Normal::ShieldBeginOverlap);
	ShieldCollision->OnComponentEndOverlap.AddDynamic(this, &AARPGShield_Normal::ShieldEndOverlap);

	ObjectType.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel10));
	IgnoreActor.Add(GetOwner());
	IgnoreActor.Add(this);

}


void AARPGShield_Normal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AARPGShield_Normal::BeginPlay()
{
	Super::BeginPlay();

}



void AARPGShield_Normal::ShieldBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != GetOwner() && OtherActor->GetOwner() != GetOwner())
	{

	}
}

void AARPGShield_Normal::ShieldEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}
