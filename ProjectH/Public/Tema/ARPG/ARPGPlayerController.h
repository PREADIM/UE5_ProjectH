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
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	virtual void BeginPlay();
	virtual void SetupInputComponent() override;
public:
	void SetLockPosition(FVector2D Pos);
	void LockOnAddViewport(bool bFlag);

	//차지 어택 위젯비율 전달
	void SetChargeAttacking(float Ratio);
	void ChargeAttackInViewport(bool bFlag);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class AARPGGameMode* GM;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class AARPGUnitBase* OwnerUnit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UUserWidget> BP_LockOnUI;
	UPROPERTY()
		class UUserWidget* LockOnUI;

	//-------------------------------------------------------
	// 메인 UI

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UARPGWidgetMain> BP_WidgetMain;
	UPROPERTY()
		class UARPGWidgetMain* WidgetMain;
};
