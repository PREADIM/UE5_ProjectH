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
				// �̹� ���� ���Ͷ� ������ ��ø ����.
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
			// ���� �и�
			AARPGUnitBase* Unit = Cast<AARPGUnitBase>(OtherActor);
			if (Unit)
			{
				//_DEBUG("Boss Overlap");
				FDamageEvent DamageEvent;
				if (Unit->bDeath != true) // ���� �� �� �ִ��� �Ǵ�
				{
					if (Unit->bParring == true && OwnerUnit->bDontParringAttack == false)
					{
						if (Unit->ParringHitFunc(OwnerUnit->GetActorLocation()))
						{
							OwnerUnit->ParringHit(Unit);
							Unit->bCanParringAttack = true;
						}
						else // �и� ����
						{
							// �ϴ� ������ �ϰ� ��ŷ���� �׾������� �˾Ƽ� �Ǵ�
							Unit->TakeDamageCalculator(this, DamageEvent, OwnerController, OwnerUnit);
							PlayWeaponSound(EWeaponSFX::AttackHitSFX);
						}
					}
					else // �и����� �ƴϰų� �и��� ���ϴ� ����
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