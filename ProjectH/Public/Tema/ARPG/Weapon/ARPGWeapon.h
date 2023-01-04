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
	class AController* GetOwnerController();

public:
	virtual void Tick(float DeltaTime);
	virtual void AttackEnd() {}
	virtual void SetWeaponCollision(bool bFlag) {}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay();



public:
	UPROPERTY()
		class AARPGUnitBase* OwnerUnit;

	// 무기마다 AP를 까는 수치가 다를 수도 있으므로 이것으로 계산
	// 공격마다의 AP를 까는 수치를 더 한다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float WeaponAP_DMG = 0.0f;
	
	// 무기마다 해당 무기를 휘두를수 있는 필요 AP
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float UseAP;
};
