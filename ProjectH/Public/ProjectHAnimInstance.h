// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Animation/AnimInstance.h"
#include "ProjectHAnimInstance.generated.h"


/*--------------------------
	Top Main AnimInstance
--------------------------*/

/**
 *
 */
UCLASS()
class PROJECTH_API UProjectHAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UProjectHAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	virtual void NativeBeginPlay();

public:
	bool bMoving();

	float SetDirection();

	UFUNCTION()
		void AnimNotify_Disable();
	UFUNCTION()
		void AnimNotify_Enable();
	UFUNCTION()
		void AnimNotify_WalkSound();

	void FootStepPlaySound(int32 SoundNum);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class AProjectHCharacter* OwnerCharacter;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UCharacterMovementComponent* MovementComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pawn)
		float CurrentSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pawn)
		bool bIsMoving;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pawn)
		float Direction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pawn)
		float Pitch;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pawn)
		bool bIsRunning;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RunSpeed)
		float CurrentAccel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RunSpeed)
		bool bIsInAir;

};
