// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "PlayerControllerBase.h"
#include "ARPGPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API AARPGPlayerController : public APlayerControllerBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		class UAudioComponent* AudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundBase* NormalSound;

public:
	AARPGPlayerController();
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	virtual void BeginPlay();
	virtual void SetupInputComponent() override;
	virtual void SetNewMouseSensitivity() override;

public:
	//차지 어택 위젯비율 전달
	void SetChargeAttacking(float Ratio);
	void ChargeAttackInViewport(bool bFlag);

	void SetBossHPWidget(bool bFlag, class AARPGEnermy* Boss);
	bool IsBossHPWidget();

	UFUNCTION(BlueprintCallable)
		void SetPlaySound(class USoundBase* Sound = nullptr);
	void SetPlayNormalSound();
	void StopSound();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class AARPGGameMode* GM;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class AARPGUnitBase* OwnerUnit;

	UPROPERTY(VisibleAnywhere)
		class UProjectHGameInstance* GI;

	//-------------------------------------------------------
	// 메인 UI

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UARPGWidgetMain> BP_WidgetMain;
	UPROPERTY()
		class UARPGWidgetMain* WidgetMain;

	// 보스 전용 HP 바 생성
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UARPGWidget_BossHPView> BP_WidgetBossHP;
	UPROPERTY()
		class UARPGWidget_BossHPView* WidgetBossHP;

	UFUNCTION()
		void BindVisibleWidget();
	UFUNCTION()
		void BindHiddenWidget();


	/*----------------
		옵션 창
	------------------*/
	UPROPERTY(EditAnywhere, Category = ESCMenu)
		TSubclassOf<class UARPGESCMenu> BP_ARPGESCMenu;
	UPROPERTY()
		class UARPGESCMenu* ARPGESCMenu;
	void OpenESC();
};

