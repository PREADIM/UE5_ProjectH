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
	//ARPGUnitChannel = ECollisionChannel::ECC_GameTraceChannel12;

	UseAP = 40.f;
}

void AARPGWeapon_Sword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AARPGWeapon_Sword::BeginPlay()
{
	Super::BeginPlay();

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

void AARPGWeapon_Sword::SetPhysics()
{
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
	float ChargeDMG = ChargeTime / MaxChargeTime;
	
	if (ChargeDMG >= 0.5)
	{
		Charge = ChargeDMG + 1.7f;
	}
	else
	{
		Charge = ChargeDMG + 1.f; // �ش� Charge�� WeaponDamage�� �������Ͽ� �� ����
	}


	return ChargeDMG;
}

void AARPGWeapon_Sword::End()
{
	Charge = 1.f;
	ChargeTime = 0.f;
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
				// �̹� ���� ���Ͷ� ������ ��ø ����.
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
						/*float TotalDamage = OwnerUnit->CalculDamage(WeaponDamage * Charge);
						float APDMG = OwnerUnit->CalculAPDamage(WeaponAP_DMG);
						Unit->TakeDamageCalculator(APDMG, TotalDamage, DamageEvent, OwnerController, this);*/
						Unit->TakeDamageCalculator(this, DamageEvent, OwnerController, OwnerUnit);
					}
				}
				else
				{
					// �ϴ� ������ �ϰ� ����ŷ���� �׾������� �˾Ƽ� �Ǵ�
					/*float TotalDamage = OwnerUnit->CalculDamage(WeaponDamage * Charge);
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

