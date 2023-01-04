// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "GameFramework/PlayerController.h"
#include "ARPGPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API AARPGPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AARPGPlayerController();
	virtual void OnPossess(APawn* InPawn);
	virtual void BeginPlay();
	virtual void SetupInputComponent() override;

	

public:
	void SetLockPosition(FVector2D Pos);
	void LockOnAddViewport(bool bFlag);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class AARPGGameMode* GM;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class AARPGUnit* OwnerUnit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UUserWidget> BP_LockOnUI;
	UPROPERTY()
		class UUserWidget* LockOnUI;
	
};
