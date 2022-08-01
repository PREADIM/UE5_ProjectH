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

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector Unit1_CameraTransform;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector Unit2_CameraTransform;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector Unit3_CameraTransform;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector Unit4_CameraTransform;


	UPROPERTY(VisibleAnywhere)
		class AJRPGPlayerController* OwnerController;
	UPROPERTY(VisibleAnywhere)
		class AJRPGGameMode* GM;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UJRPGPartySettingUI> BP_PartySettingUI;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UJRPGPartySettingUI* PartySettingUI; // ��Ƽ �����ϴ� UI

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UJRPGSettingPartySlot> BP_SettingUI;
	UPROPERTY(VisibleAnywhere)
		class UJRPGSettingPartySlot* SettingUI;


	/* ĳ���� ��Ƽ����â�� ���̳����ϰ� Ȱ���ϱ� ���� ������. �ڡ� */
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	class APartySettingField* PartySettingField;
	UPROPERTY()
		TArray<class AJRPGUnit*> SpawnChars;
	UPROPERTY(VisibleAnywhere)
		class AActor* PartySettingCamera; // ��Ƽ �����ҋ� �ʿ��� ī�޶� 

	UPROPERTY()
		TArray<int32> CurrentParty;

	UPROPERTY(EditAnywhere)
		FVector ZeroLocation;

	FVector TargetLocation;
	FVector CurrentLocation;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	void Init(class UJRPGMainWidget* OwnerMainUI);

	UFUNCTION()
		void LMB();

	void SetCurrentParty();
	void SetSpawnUnit(int32 Number);

	void SettingPartySlot(); // ��Ƽ �����ϴ� UI ����.
	void ResomeUI(); // �ٽ� ��ó�� ��Ƽ UI �������� ���ư���.
};
