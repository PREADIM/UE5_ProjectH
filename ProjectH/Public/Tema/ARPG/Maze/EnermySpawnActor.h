// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "GameFramework/Actor.h"
#include "EnermySpawnActor.generated.h"

UCLASS()
class PROJECTH_API AEnermySpawnActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnermySpawnActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
		class USphereComponent* Collision;

	UPROPERTY(EditAnywhere)
		TArray<TSubclassOf<class AARPGEnermy>> BP_Enermys;

	UPROPERTY(VisibleAnywhere)
		class AARPGEnermy* SpawnEnermy;

	FVector DefaultLocation;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UFUNCTION()
		void OverlapUnit(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OverlapEndUnit(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);



	void SpawnRandEnermy();
};
