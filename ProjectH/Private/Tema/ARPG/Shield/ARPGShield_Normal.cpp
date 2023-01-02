// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/Shield/ARPGShield_Normal.h"
#include "Tema/ARPG/ARPGUnitBase.h"

AARPGShield_Normal::AARPGShield_Normal()
{
	PrimaryActorTick.bCanEverTick = false;

	ShieldMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ShieldMesh"));

	ShieldCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("ShieldCollision"));
	ShieldCollision->SetupAttachment(ShieldMesh);

	ShieldCollision->OnComponentBeginOverlap.AddDynamic(this, &AARPGShield_Normal::ShieldBeginOverlap);

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

void AARPGShield_Normal::BlockEnd()
{
	SetHitEndActor();
}


void AARPGShield_Normal::ShieldBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (!HitEndActor.IsEmpty())
	{
		for (auto Hit : HitEndActor)
		{
			if (OtherActor == Hit)
			{
				_DEBUG("Shield Return");
				return;
				// 이미 닿은 액터라 데미지 중첩 방지.
			}
		}
	}

	if (OtherActor != GetOwner() && OtherActor->GetOwner() != GetOwner())
	{
		if (OwnerUnit)
		{
			OwnerUnit->bHitting = true;
		}
	}
}

