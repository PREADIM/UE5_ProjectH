// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Tema/ARPG/ARPGShield.h"
#include "ARPGShield_Normal.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API AARPGShield_Normal : public AARPGShield
{
	GENERATED_BODY()

public:
	AARPGShield_Normal();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void ShieldBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void ShieldEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay();

private:
	UPROPERTY()
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectType;
	UPROPERTY()
		TArray<AActor*> IgnoreActor; // 무시할 액터
	UPROPERTY()
		TArray<AActor*> HitEndActor;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		class USkeletalMeshComponent* ShieldMesh;

	
};
