// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "GameFramework/Character.h"
#include "JRPGUnitBase.generated.h"

UCLASS()
class PROJECTH_API AJRPGUnitBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AJRPGUnitBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
