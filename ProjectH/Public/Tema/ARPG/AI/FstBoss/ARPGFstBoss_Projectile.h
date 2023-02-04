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
	// �� ���ʹ� 2�ʰ� ����ǰ� ����Ʈ�� 1.5�� �� ����Ǳ⶧���� ����Ʈ������� �ݸ��� ����

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UCapsuleComponent* Collision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UProjectileMovementComponent* Projectile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float EndCollisionTime;


};
