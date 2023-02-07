// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/Weapon/ARPGWeapon.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AARPGWeapon::AARPGWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ARPGUnitChannel = ECollisionChannel::ECC_GameTraceChannel12;
}

void AARPGWeapon::PlayWeaponSound(EWeaponSFX WeaponSFX)
{
	if(WeaponSFXs.Find(WeaponSFX))
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), WeaponSFXs[WeaponSFX], GetActorLocation());
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

AController* AARPGWeapon::GetOwnerController()
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr) 
		return nullptr;

	return OwnerPawn->GetController();
}




