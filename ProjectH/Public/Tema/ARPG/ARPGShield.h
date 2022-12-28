// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "GameFramework/Actor.h"
#include "ARPGShield.generated.h"

UCLASS()
class PROJECTH_API AARPGShield : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AARPGShield();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay();


public:
	UPROPERTY()
		class AActor* OwnerUnit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		class UBoxComponent* ShieldCollision;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime);

};
