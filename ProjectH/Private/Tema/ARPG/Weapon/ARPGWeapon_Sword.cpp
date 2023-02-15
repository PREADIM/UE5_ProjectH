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

	UseAP = 40.f;
}

void AARPGWeapon_Sword::BeginPlay()
{
	Super::BeginPlay();

	OwnerController = GetOwnerController();
	WeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &AARPGWeapon_Sword::SwordBeginOverlap);

	if (OwnerUnit)
	{
		OwnerUnit->OnChargeAttackInit.AddUFunction(this, FName("ChargeAttackInit"));
	}
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
	}
	else
	{
		WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AARPGWeapon_Sword::SetPhysics()
{
	Super::SetPhysics();
	SwordMesh->SetSimulatePhysics(true);
	SwordMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AARPGWeapon_Sword::SetOwnerNoSee(bool bFlag)
{
	if (bFlag)
	{
		SwordMesh->SetOwnerNoSee(true);
		SwordMesh->SetCastShadow(true);
		SwordMesh->SetCastHiddenShadow(true);
	}
	else
	{
		SwordMesh->SetOnlyOwnerSee(true);
		SwordMesh->SetCastShadow(false);
		SetWeaponCollision(false);
	}

}

bool AARPGWeapon_Sword::IsChargeAttack()
{	
	return true;
}

float AARPGWeapon_Sword::ChargeAttack(float DeltaSeconds)
{
	ChargeTime += DeltaSeconds;
	ChargeRatio = FMath::Clamp(ChargeTime / MaxChargeTime, 0.f, 1.0f);
	
	if (ChargeRatio >= 0.5)
	{
		Charge = ChargeRatio + 6.0f;
	}
	else
	{
		Charge = ChargeRatio + 1.5f; // 해당 Charge로 WeaponDamage에 곱연산하여 딜 증가
	}


	return ChargeRatio;
}

void AARPGWeapon_Sword::ChargeAttackInit()
{
	Super::ChargeAttackInit();
}

void AARPGWeapon_Sword::WeaponAttackEnd()
{
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

	if (OtherActor != OwnerUnit && OtherActor->GetOwner() != OwnerUnit->GetOwner())
	{
		TArray<AActor*> OutActors;

		/*bool bOverlap = UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), SphereRadius, ObjectType, nullptr, IgnoreActor, OutActors);
		DrawDebugSphere(GetWorld(), GetActorLocation(), SphereRadius, 20, bOverlap ? FColor::Green : FColor::Red, false, 4.0f);
		if (bOverlap)
		{
			
		}*/

		if (OtherComp->GetCollisionObjectType() == ARPGUnitChannel)
		{
			// 공통 분모
			AARPGUnitBase* Unit = Cast<AARPGUnitBase>(OtherActor);
			if (Unit)
			{
				FDamageEvent DamageEvent;
				if (Unit->bDeath != true) // 공격 할 수 있는지 판단
				{
					if (Unit->bParring == true && OwnerUnit->bDontParringAttack == false)
					{
						if (Unit->ParringHitFunc(OwnerUnit->GetActorLocation()))
						{
							OwnerUnit->ParringHit(Unit);
							PlayWeaponSound(EWeaponSFX::ParringHitSFX);
							Unit->bCanParringAttack = true;
						}
						else // 패링 실패
						{
							// 일단 공격을 하고 블럭킹인지 죽었는지는 알아서 판단
							Unit->TakeDamageCalculator(this, DamageEvent, OwnerController, OwnerUnit);
							PlayWeaponSound(EWeaponSFX::AttackHitSFX);
						}
					}
					else
					{
						// 일단 공격을 하고 블럭킹인지 죽었는지는 알아서 판단
						Unit->TakeDamageCalculator(this, DamageEvent, OwnerController, OwnerUnit);
						PlayWeaponSound(EWeaponSFX::AttackHitSFX);
					}
				}

				HitEndActor.AddUnique(OtherActor);
				return;
			}
		}
	}
}


