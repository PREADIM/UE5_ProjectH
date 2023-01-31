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
}

void AARPG_FstBossWeapon::SetWeaponCollision(bool bFlag)
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


void AARPG_FstBossWeapon::End()
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
							// �ϴ� ������ �ϰ� ����ŷ���� �׾������� �˾Ƽ� �Ǵ�
							/*float TotalDamage = OwnerUnit->CalculDamage(WeaponDamage);
							float APDMG = OwnerUnit->CalculAPDamage(WeaponAP_DMG);
							Unit->TakeDamageCalculator(APDMG, TotalDamage, DamageEvent, OwnerController, this);*/
							Unit->TakeDamageCalculator(this, DamageEvent, OwnerController, OwnerUnit);
						}
					}
					else
					{
						// �ϴ� ������ �ϰ� ����ŷ���� �׾������� �˾Ƽ� �Ǵ�
						/*float TotalDamage = OwnerUnit->CalculDamage(WeaponDamage);
						float APDMG = OwnerUnit->CalculAPDamage(WeaponAP_DMG);
						Unit->TakeDamageCalculator(APDMG, TotalDamage, DamageEvent, OwnerController, this);*/
						Unit->TakeDamageCalculator(this, DamageEvent, OwnerController, OwnerUnit);
					}
				}

				HitEndActor.AddUnique(OtherActor);
				//_DEBUG("Overlap Enermy Actor : %s", *OtherActor->GetName());
				return;
			}
		}
	}
}