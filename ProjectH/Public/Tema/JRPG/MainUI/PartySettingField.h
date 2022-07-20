// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "GameFramework/Pawn.h"
#include "PartySettingField.generated.h"

UCLASS()
class PROJECTH_API APartySettingField : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APartySettingField();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Root)
		class USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
		class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Unit)
		class USceneComponent* Unit1;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Unit)
		class USceneComponent* Unit2;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Unit)
		class USceneComponent* Unit3;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Unit)
		class USceneComponent* Unit4;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UnitCamera)
		class UCameraComponent* Unit1_Camera;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UnitCamera)
		class UCameraComponent* Unit2_Camera;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UnitCamera)
		class UCameraComponent* Unit3_Camera;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UnitCamera)
		class UCameraComponent* Unit4_Camera;





protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
