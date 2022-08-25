// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "GameFramework/Pawn.h"
#include "JRPGCamera.generated.h"

UCLASS()
class PROJECTH_API AJRPGCamera : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AJRPGCamera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Arm)
		class USceneComponent* Root;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Arm)
		class USpringArmComponent* SpringArm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
		class UCameraComponent* Camera;

public:
	UPROPERTY(VisibleAnywhere)
		FVector TargetLocation;
	UPROPERTY(VisibleAnywhere)
		FVector CurrentLocation;

	UPROPERTY(VisibleAnywhere)
		FRotator TargetRotation;
	UPROPERTY(VisibleAnywhere)
		FRotator CurrentRotation;

	UPROPERTY(VisibleAnywhere)
		class AJRPGPlayerController* OwnerController;
	UPROPERTY(VisibleAnywhere)
		class ABattleField* CurrentField;
	


};
