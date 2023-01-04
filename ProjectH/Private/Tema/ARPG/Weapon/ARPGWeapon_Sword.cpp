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
				// �̹� ���� ���Ͷ� ������ ��ø ����.
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
			// �Ⱦ�.
		}

		if (OtherComp->GetCollisionObjectType() == ARPGUnitChannel)
		{
			// ���� �и�
			AARPGUnitBase* Unit = Cast<AARPGUnitBase>(OtherActor);
			if (Unit)
			{
				FDamageEvent DamageEvent;
				if (Unit->CanThisDamage()) // ���� �� �� �ִ��� �Ǵ�
				{
					float TotalDamage = OwnerUnit->CalculDamage(WeaponDamage);
					Unit->TakeDamage(TotalDamage, DamageEvent, OwnerController, this);
				}
				else // �������� �������� ���� ������� �̷����ϸ� �ڵ带 ���� �����ϰ� �������� ��밡��
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


