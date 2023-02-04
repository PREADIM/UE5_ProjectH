// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Tema/ARPG/Weapon/ARPGWeapon.h"
#include "ARPG_FstBossWeapon.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API AARPG_FstBossWeapon : public AARPGWeapon
{
	GENERATED_BODY()

public:
	AARPG_FstBossWeapon();

	virtual void AttackEnd() override;
	virtual void SetWeaponCollision(bool bFlag) override;
	virtual void End() override;

	UFUNCTION()
		void WeaponBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
		class UCapsuleComponent* WeaponCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Controller")
		class AController* OwnerController;

};
