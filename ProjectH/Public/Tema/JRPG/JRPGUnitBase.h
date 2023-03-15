// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "GameFramework/Character.h"
#include "PhysicalSoundStruct.h"
#include "JRPGUnitBase.generated.h"

UCLASS()
class PROJECTH_API AJRPGUnitBase : public ACharacter
{
	GENERATED_BODY()

public:
	AJRPGUnitBase();

protected:
	virtual void BeginPlay();

public:	
	virtual void Tick(float DeltaTime);
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent);

	/*-----------------------
			피지컬 사운드
	------------------------*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PhysicalSound)
		TMap<TEnumAsByte<EPhysicalSurface>, FPhysicalSoundStruct> PhysicalAllSounds;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = PhysicalSound)
		FPhysicalSoundStruct PhysicalSounds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PhysicalSound)
		float SurfaceDistance = 300.f; // 땅끝의 거리
	void SetPhysicalSound();

};
