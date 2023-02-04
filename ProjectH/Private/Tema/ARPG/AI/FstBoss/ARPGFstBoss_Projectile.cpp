// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/AI/FstBoss/ARPGFstBoss_Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraComponent.h"
#include "Tema/ARPG/ARPGUnitBase.h"

// Sets default values
AARPGFstBoss_Projectile::AARPGFstBoss_Projectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Collision = CreateDefaultSubobject<class UCapsuleComponent>(TEXT("Collision"));
	RootComponent = Collision;

	
	Projectile = CreateDefaultSubobject<class UProjectileMovementComponent>(TEXT("Projectile"));
	Projectile->InitialSpeed = 1000.f;
	Projectile->Bounciness = 0.f;
	Projectile->Friction = 0.f;
	Projectile->BounceVelocityStopSimulatingThreshold = 0.f;

	InitialLifeSpan = 2.f;
	EndCollisionTime = 1.0f;	
}

// Called when the game starts or when spawned
void AARPGFstBoss_Projectile::BeginPlay()
{
	Super::BeginPlay();

	OwnerUnit = Cast<AARPGUnitBase>(GetOwner());
	Collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	Collision->OnComponentBeginOverlap.AddDynamic(this, &AARPGFstBoss_Projectile::OverlapProjectile);
	FTimerHandle CollisionOffHandle;
	GetWorld()->GetTimerManager().SetTimer(CollisionOffHandle, this, &AARPGFstBoss_Projectile::CollisionOff, EndCollisionTime, false);
}

void AARPGFstBoss_Projectile::OverlapProjectile(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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
	{
		_DEBUG("Projectile Not OwnerUnit");
		return;
	}


	_DEBUG("Projectile");

	if (OtherActor != OwnerUnit && OtherActor->GetOwner() != OwnerUnit->GetOwner())
	{
		TArray<AActor*> OutActors;

		if (OtherComp->GetCollisionObjectType() == ARPGUnitChannel)
		{
			// 공통 분모
			AARPGUnitBase* Unit = Cast<AARPGUnitBase>(OtherActor);
			if (Unit)
			{
				FDamageEvent DamageEvent;
				if (Unit->bDeath != true) // 공격 할 수 있는지 판단
				{		
					//_DEBUG("Projectile Hit");
					// 일단 공격을 하고 블럭킹인지 죽었는지는 알아서 판단
					Unit->TakeDamageCalculator(this, DamageEvent, OwnerUnit->GetController(), OwnerUnit);
				}

				HitEndActor.AddUnique(OtherActor);
				return;
			}
		}
	}


}

void AARPGFstBoss_Projectile::CollisionOff()
{
	Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


