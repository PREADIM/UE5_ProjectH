// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/Weapon/ARPGWeapon_Sword.h"
#include "DrawDebugHelpers.h"
#include "Tema/ARPG/ARPGUnitBase.h"

AARPGWeapon_Sword::AARPGWeapon_Sword()
{
	PrimaryActorTick.bCanEverTick = false;
	SwordMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwordMesh"));

	WeaponCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("SwordCollision"));
	WeaponCollision->SetupAttachment(SwordMesh);

	WeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &AARPGWeapon_Sword::SwordBeginOverlap);
	ARPGUnitChannel = ECollisionChannel::ECC_GameTraceChannel12;

	SphereRadius = 20.f;
}


void AARPGWeapon_Sword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AARPGWeapon_Sword::BeginPlay()
{
	Super::BeginPlay();

	ObjectType.Add(UEngineTypes::ConvertToObjectType(ARPGUnitChannel));

	IgnoreActor.Add(GetOwner());
	IgnoreActor.Add(this);

	OwnerController = GetOwnerController();
}


void AARPGWeapon_Sword::AttackEnd()
{
	SetHitEndActor();
}

void AARPGWeapon_Sword::SetWeaponCollision(bool bFlag)
{
	if (bFlag)
	{
		WeaponCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		//_DEBUG("Collision On");
	}
	else
	{
		WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//_DEBUG("Collision Off");
	}
}

void AARPGWeapon_Sword::SwordBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HitEndActor.IsEmpty())
	{
		for (auto Hit : HitEndActor)
		{
			if (OtherActor == Hit)
			{
				return;
				// 이미 닿은 액터라 데미지 중첩 방지.
			}
		}
	}

	if (!OwnerUnit)
		return;

	if (OtherActor != OwnerUnit && OtherActor->GetOwner() != OwnerUnit)
	{
		TArray<AActor*> OutActors;

		bool bOverlap = UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), SphereRadius, ObjectType, nullptr, IgnoreActor, OutActors);
		DrawDebugSphere(GetWorld(), GetActorLocation(), SphereRadius, 20, bOverlap ? FColor::Green : FColor::Red, false, 4.0f);
		if (bOverlap)
		{
			// 안씀.
		}

		if (OtherComp->GetCollisionObjectType() == ARPGUnitChannel)
		{
			// 공통 분모
			AARPGUnitBase* Unit = Cast<AARPGUnitBase>(OtherActor);
			if (Unit)
			{
				FDamageEvent DamageEvent;
				if (Unit->CanThisDamage()) // 공격 할 수 있는지 판단
				{
					float TotalDamage = OwnerUnit->CalculDamage(WeaponDamage);
					Unit->TakeDamage(TotalDamage, DamageEvent, OwnerController, this);
				}
				else // 정상적인 데미지가 들어가지 않을경우 이렇게하면 코드를 보다 간결하고 공통으로 사용가능
				{
					float APDMG = OwnerUnit->CalculAPDamage(WeaponAP_DMG);
					Unit->TakeDamageAP(WeaponAP_DMG);
				}

				HitEndActor.AddUnique(OtherActor);
				//_DEBUG("Overlap Enermy Actor : %s", *OtherActor->GetName());
				return;
			}
		}
	}
}


