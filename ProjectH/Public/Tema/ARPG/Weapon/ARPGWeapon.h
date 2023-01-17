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
	virtual void SetPhysics() {}
	virtual void SetOwnerNoSee(bool bFlag) {}
	
	// ���� ������ �������� ���� Ȯ���ϰ� true�� float������ ���� �󸶳� �����Ǿ����� �˷��ش�.
	virtual bool IsChargeAttack() { return false; } // ���⸶�� ���������� ���� ���� ���� ���� �ִ�.
	virtual float ChargeAttack(float DeltaSeconds) { return 0.f; }

	// ������ ����Ǿ����� ���⸶�� �ʱ�ȭ �ؾ��ϴ� ��ġ�� �ִٸ� �� �Լ��� �̿�.
	virtual void EndAttack() {}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay();



public:
	UPROPERTY()
		class AARPGUnitBase* OwnerUnit;

	// ���⸶�� AP�� ��� ��ġ�� �ٸ� ���� �����Ƿ� �̰����� ���
	// ���ݸ����� AP�� ��� ��ġ�� �� �Ѵ�.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float WeaponAP_DMG = 0.0f;
	
	// ���⸶�� �ش� ���⸦ �ֵθ��� �ִ� �ʿ� AP
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float UseAP;
};
