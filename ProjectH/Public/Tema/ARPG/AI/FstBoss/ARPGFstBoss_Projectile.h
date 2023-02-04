// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Tema/ARPG/Weapon/ARPGWeapon.h"
#include "ARPGFstBoss_Projectile.generated.h"

UCLASS()
class PROJECTH_API AARPGFstBoss_Projectile : public AARPGWeapon
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AARPGFstBoss_Projectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	UFUNCTION()
		void OverlapProjectile(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void CollisionOff(); 
	// 이 액터는 2초간 진행되고 이펙트는 1.5초 간 진행되기때문에 이펙트사라질때 콜리전 오프

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UCapsuleComponent* Collision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UProjectileMovementComponent* Projectile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float EndCollisionTime;


};
