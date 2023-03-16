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


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector Unit1_CameraTransform;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector Unit2_CameraTransform;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector Unit3_CameraTransform;

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
	UPROPERTY()
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	UPROPERTY()
		class APartySettingField* PartySettingField;
	UPROPERTY()
		TArray<class AJRPGUnit*> SpawnChars;
	UPROPERTY(VisibleAnywhere)
		class AActor* PartySettingCamera; // ��Ƽ �����ҋ� �ʿ��� ī�޶� 

	UPROPERTY()
		TArray<int32> CurrentParty;

	UPROPERTY(EditAnywhere)
		FVector ZeroLocation;

	UPROPERTY(VisibleAnywhere)
		bool bOpenUI; // SettingPartySlot�� ���������� LMB�� �ϸ�ȵǱ⶧��.

	UPROPERTY()
		TArray<class UJRPGSettingPartyIcon*> CurrentPartyIcons;
	// ���Ŀ� 1 2 3�� ���� ��Ƽ ��� �Ҵ�Ǿ��ִ��� ��ȣ�� ���϶� ���� �ڸ��� �����Ҷ� ��ȣ�� �ٲܶ� �����ҵ�
	// ���� ���õ� �����ܵ��� ���� ���� �ִ� �Ҽ� �ְ� �ϴ� �迭


	FVector TargetLocation;
	FVector CurrentLocation;
	int32 SelectNumber; // ���° ĭ�� ���� �ߴ°�?
	int32 SwapCharFieldNum; // ���° ĭ�� �ٲ��� �ϴ°�?


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

	void NextChar(int32 Number); // ĳ���� ���ÿ��� ������ �ѱ�� �Լ�.	

	void SetCurrentParty();
	void SpawnCharacter();
	void SetSpawnUnit(int32 Number, int32 CharNum);
	void SetPartyList(int32 CharNum, class UJRPGSettingPartyIcon* OwnerIcon); // ��Ƽ ĳ������ ����� ����Ǿ����Ƿ�, �ٽ� ����

	UFUNCTION()
		void SetPartyChar(); // ��Ƽ ĳ������ ����� ����Ǿ����Ƿ�, ĳ���� ��ġ �缳��.
	void SwapIconColor(); // ������ �÷� �ٲٱ�.

	void SettingPartySlot(); // ��Ƽ �����ϴ� UI ����.
	void ResomeUI(); // �ٽ� ��ó�� ��Ƽ UI �������� ���ư���.
};
