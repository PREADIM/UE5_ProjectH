// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/AI/FstBoss/ARPG_FstBossWeapon.h"
#include "Tema/ARPG/ARPGUnitBase.h"

AARPG_FstBossWeapon::AARPG_FstBossWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("SwordCollision"));

	WeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &AARPG_FstBossWeapon::WeaponBeginOverlap);
	ARPGUnitChannel = ECollisionChannel::ECC_GameTraceChannel12;
}


void AARPG_FstBossWeapon::BeginPlay()
{
	Super::BeginPlay();
	OwnerController = GetOwnerController();
}


void AARPG_FstBossWeapon::AttackEnd()
{
	SetHitEndActor();
	_DEBUG("FstBossWeapon AttackEnd");
}

void AARPG_FstBossWeapon::SetWeaponCollision(bool bFlag)
{
	if (bFlag)
	{
		WeaponCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);	
	}
	else
	{
		WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}


void AARPG_FstBossWeapon::WeaponAttackEnd()
{
}

void AARPG_FstBossWeapon::WeaponBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HitEndActor.IsEmpty())
	{
		for (auto Hit : HitEndActor)
		{
			if (OtherActor == Hit)
			{
				//_DEBUG("Return HitEndActor");
				return;
				// 이미 닿은 액터라 데미지 중첩 방지.
			}
		}
	}

	if (!OwnerUnit)
		return;

	if (OtherActor != OwnerUnit && OtherActor->GetOwner() != OwnerUnit->GetOwner())
	{
		TArray<AActor*> OutActors;

		if (OtherComp->GetCollisionObjectType() == ARPGUnitChannel)
		{
			// 공통 분모
			AARPGUnitBase* Unit = Cast<AARPGUnitBase>(OtherActor);
			if (Unit)
			{
				//_DEBUG("Boss Overlap");
				FDamageEvent DamageEvent;
				if (Unit->bDeath != true) // 공격 할 수 있는지 판단
				{
					if (Unit->bParring == true && OwnerUnit->bDontParringAttack == false)
					{
						if (Unit->ParringHitFunc(OwnerUnit->GetActorLocation()))
						{
							OwnerUnit->ParringHit(Unit);
							Unit->bCanParringAttack = true;
						}
						else // 패링 실패
						{
							// 일단 공격을 하고 블럭킹인지 죽었는지는 알아서 판단
							Unit->TakeDamageCalculator(this, DamageEvent, OwnerController, OwnerUnit);
							PlayWeaponSound(EWeaponSFX::AttackHitSFX);
						}
					}
					else // 패링중이 아니거나 패링을 못하는 공격
					{
						Unit->TakeDamageCalculator(this, DamageEvent, OwnerController, OwnerUnit);
						PlayWeaponSound(EWeaponSFX::AttackHitSFX);
					}
				}

				HitEndActor.AddUnique(OtherActor);
			}
		}
	}
}