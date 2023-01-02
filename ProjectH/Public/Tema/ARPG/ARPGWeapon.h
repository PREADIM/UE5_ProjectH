// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "GameFramework/Actor.h"
#include "ARPGWeapon.generated.h"

UCLASS()
class PROJECTH_API AARPGWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AARPGWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay();

public:
	UPROPERTY()
		class AARPGUnitBase* OwnerUnit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
		class UCapsuleComponent* WeaponCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float WeaponAP_DMG = 0.0f;
	// ���⸶�� AP�� ��� ��ġ�� �ٸ� ���� �����Ƿ� �̰����� ���
	// ���ݸ����� AP�� ��� ��ġ�� �� �Ѵ�.


public:	
	// Called every frame
	virtual void Tick(float DeltaTime);

	class AController* GetOwnerController();

	virtual void AttackEnd() {}
};
