// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Tema/ARPG/Weapon/ARPGWeapon.h"
#include "ARPGShield.generated.h"

UCLASS()
class PROJECTH_API AARPGShield : public AARPGWeapon
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AARPGShield();

public:
	virtual void Tick(float DeltaTime) override;
	virtual void AttackEnd() override {}
	virtual void SetWeaponCollision(bool bFlag) override;
	virtual void SetPhysics() override;
	virtual void SetOwnerNoSee(bool bFlag) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay();


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		class UBoxComponent* ShieldCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		class USkeletalMeshComponent* ShieldMesh;

};
