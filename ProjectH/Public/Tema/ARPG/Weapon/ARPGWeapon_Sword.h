// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Tema/ARPG/ARPGWeapon.h"
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

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void AttackEnd() override;

	UFUNCTION()
		void SwordBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION(BlueprintCallable)
		void SetHitEndActor() { HitEndActor.Empty(); } // �����͵� �ʱ�ȭ.

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY()
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectType;
	UPROPERTY()
		TArray<AActor*> IgnoreActor; // ������ ����
	UPROPERTY()
		TArray<AActor*> HitEndActor;

	UPROPERTY(VisibleAnywhere)
		float SphereRadius; //SphereOverlapActors �� �ݰ�

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		class UStaticMeshComponent* SwordMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Controller")
		class AController* OwnerController;



	//----------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponDamage")
		float WeaponDamage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "WeaponDamage")
		float TotalDamage;

	//-------------------------------------------
	ECollisionChannel ARPGUnitChannel;
	ECollisionChannel ARPGShieldChannel;

};
