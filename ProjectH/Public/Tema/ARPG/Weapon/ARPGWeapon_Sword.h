// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Tema/ARPG/Weapon/ARPGWeapon.h"
#include "ARPGWeapon_Sword.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API AARPGWeapon_Sword : public AARPGWeapon
{
	GENERATED_BODY()

public:
	AARPGWeapon_Sword();

	virtual void AttackEnd() override;
	virtual void SetWeaponCollision(bool bFlag) override;
	virtual void SetPhysics() override;
	virtual void SetOwnerNoSee(bool bFlag) override;
	virtual void SetWeaponNoSee() override;

	virtual bool IsChargeAttack() override;
	virtual void ChargeAttackInit() override;
	virtual float ChargeAttack(float DeltaSeconds) override;

	virtual void WeaponAttackEnd() override;

	UFUNCTION()
		void SwordBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		class UStaticMeshComponent* SwordMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
		class UCapsuleComponent* WeaponCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Controller")
		class AController* OwnerController;

};
