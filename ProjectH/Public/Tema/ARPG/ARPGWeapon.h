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
	// 무기마다 AP를 까는 수치가 다를 수도 있으므로 이것으로 계산
	// 공격마다의 AP를 까는 수치를 더 한다.


public:	
	// Called every frame
	virtual void Tick(float DeltaTime);

	class AController* GetOwnerController();

	virtual void AttackEnd() {}
};
