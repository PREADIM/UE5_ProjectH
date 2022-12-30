// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/Weapon/ARPGWeapon_Sword.h"
#include "DrawDebugHelpers.h"
#include "Tema/ARPG/ARPGUnitBase.h"

AARPGWeapon_Sword::AARPGWeapon_Sword()
{
	SwordMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwordMesh"));

	WeaponCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("SwordCollision"));
	WeaponCollision->SetupAttachment(SwordMesh);

	WeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &AARPGWeapon_Sword::SwordBeginOverlap);
	ARPGUnitChannel = ECollisionChannel::ECC_GameTraceChannel12;
	ARPGShieldChannel = ECollisionChannel::ECC_GameTraceChannel11;

	SphereRadius = 50.f;
}


void AARPGWeapon_Sword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AARPGWeapon_Sword::BeginPlay()
{
	Super::BeginPlay();

	ObjectType.Add(UEngineTypes::ConvertToObjectType(ARPGUnitChannel));
	ObjectType.Add(UEngineTypes::ConvertToObjectType(ARPGShieldChannel));

	IgnoreActor.Add(GetOwner());
	IgnoreActor.Add(this);

	OwnerController = GetOwnerController();
}


void AARPGWeapon_Sword::AttackEnd()
{
	SetHitEndActor();
	_DEBUG("Weapon AttackEnd");
}

void AARPGWeapon_Sword::SwordBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HitEndActor.IsEmpty())
	{
		for (auto Hit : HitEndActor)
		{
			if (OtherActor == Hit)
			{
				_DEBUG("Return");
				return; 
				// 이미 닿은 액터라 데미지 중첩 방지.
			}
		}
	}

	if (OtherActor != OwnerUnit && OtherActor->GetOwner() != OwnerUnit)
	{
		TArray<AActor*> OutActors;
		
		bool bOverlap = UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), SphereRadius, ObjectType, nullptr, IgnoreActor, OutActors);
		DrawDebugSphere(GetWorld(), GetActorLocation(), SphereRadius, 20, bOverlap ? FColor::Green : FColor::Red, false, 4.0f);
		if (bOverlap)
		{
		}

		if (OtherComp->GetCollisionObjectType() == ARPGUnitChannel)
		{
			// 공통 분모
			AARPGUnitBase* Unit = Cast<AARPGUnitBase>(OtherActor);
			if (Unit)
			{
				if (Unit->bBlocking != true)
				{
					if (OwnerUnit)
					{
						FDamageEvent DamageEvent;
						TotalDamage = WeaponDamage + OwnerUnit->UnitState.ATK;
						OtherActor->TakeDamage(TotalDamage, DamageEvent, GetOwnerController(), this);
					}
				}

				HitEndActor.AddUnique(OtherActor);
				_DEBUG("Overlap Enermy Actor : %s", *OtherActor->GetName());
				return;
			}
		}
		else if(OtherComp->GetCollisionObjectType() == ARPGShieldChannel)
		{
			HitEndActor.AddUnique(OtherActor);
			_DEBUG("Shield Overlap");
		}
	}
}


