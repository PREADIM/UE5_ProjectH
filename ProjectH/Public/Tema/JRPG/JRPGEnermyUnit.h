// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tema/JRPG/JRPGUnit.h"
#include "JRPGEnermyUnit.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API AJRPGEnermyUnit : public AJRPGUnit
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void PostInitializeComponents() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BattleTurnStart() override;
	virtual void UnitBattleStart() override;
	virtual void DeadBattleListRemove() override;	
};
