// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Animation/AnimInstance.h"
#include "ARPG_EnermyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UARPG_EnermyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UARPG_EnermyAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	

public:
	float SetDircection(class APawn* Pawn);

public:
	UPROPERTY(BlueprintReadWrite)
		class AARPGEnermy* CurrentCharacter;

	UPROPERTY(BlueprintReadWrite)
		class APawn* OwnerPawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bInAir;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bBlocking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bBattleMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Direction;
	
};
