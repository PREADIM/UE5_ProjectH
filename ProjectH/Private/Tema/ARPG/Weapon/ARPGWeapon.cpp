// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/Weapon/ARPGWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "Tema/ARPG/ARPGUnitBase.h"

// Sets default values
AARPGWeapon::AARPGWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ARPGUnitChannel = ECollisionChannel::ECC_GameTraceChannel12;
}

void AARPGWeapon::PlayWeaponSound(EWeaponSFX WeaponSFX)
{
	if (WeaponSFX == EWeaponSFX::SwingSFX)
	{
		if (ChargeRatio >= 0.5f)
			WeaponSFX = EWeaponSFX::ChargeSwingSFX;
	}

	if(WeaponSFXs.Find(WeaponSFX))
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), WeaponSFXs[WeaponSFX].Sound, GetActorLocation(), 1.f, 1.f, 1.f, WeaponSFXs[WeaponSFX].Attenuation);
}

// Called when the game starts or when spawned
void AARPGWeapon::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AARPGWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AARPGWeapon::SetPhysics()
{
	PlayWeaponSound(EWeaponSFX::DropSFX);
	FTimerHandle DestroyHandle;
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(DestroyHandle, FTimerDelegate::CreateLambda([&]()
			{
				Destroy();
			}), 10.f, false);
	}
}

void AARPGWeapon::ChargeAttackInit()
{
	Charge = 1.f;
	ChargeTime = 0.f;
	ChargeRatio = 0.f;
}

AController* AARPGWeapon::GetOwnerController()
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr) 
		return nullptr;

	return OwnerPawn->GetController();
}




