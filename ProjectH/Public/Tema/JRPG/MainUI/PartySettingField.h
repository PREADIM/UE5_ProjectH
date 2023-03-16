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
		class UJRPGPartySettingUI* PartySettingUI; // 파티 셋팅하는 UI

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UJRPGSettingPartySlot> BP_SettingUI;
	UPROPERTY(VisibleAnywhere)
		class UJRPGSettingPartySlot* SettingUI;


	/* 캐릭터 파티셋팅창을 다이나믹하게 활용하기 위한 변수들. ★★ */
	UPROPERTY()
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	UPROPERTY()
		class APartySettingField* PartySettingField;
	UPROPERTY()
		TArray<class AJRPGUnit*> SpawnChars;
	UPROPERTY(VisibleAnywhere)
		class AActor* PartySettingCamera; // 파티 설정할떄 필요한 카메라 

	UPROPERTY()
		TArray<int32> CurrentParty;

	UPROPERTY(EditAnywhere)
		FVector ZeroLocation;

	UPROPERTY(VisibleAnywhere)
		bool bOpenUI; // SettingPartySlot이 켜져있으면 LMB를 하면안되기때문.

	UPROPERTY()
		TArray<class UJRPGSettingPartyIcon*> CurrentPartyIcons;
	// 추후에 1 2 3번 같은 파티 어디에 할당되어있는지 번호를 붙일때 서로 자리를 스왑할때 번호를 바꿀때 유용할듯
	// 현재 선택된 아이콘들의 색을 껏다 켯다 할수 있게 하는 배열


	FVector TargetLocation;
	FVector CurrentLocation;
	int32 SelectNumber; // 몇번째 칸을 선택 했는가?
	int32 SwapCharFieldNum; // 몇번째 칸이 바뀌어야 하는가?


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

	void NextChar(int32 Number); // 캐릭터 세팅에서 옆으로 넘기기 함수.	

	void SetCurrentParty();
	void SpawnCharacter();
	void SetSpawnUnit(int32 Number, int32 CharNum);
	void SetPartyList(int32 CharNum, class UJRPGSettingPartyIcon* OwnerIcon); // 파티 캐릭터의 목록이 변경되었으므로, 다시 설정

	UFUNCTION()
		void SetPartyChar(); // 파티 캐릭터의 목록이 변경되었으므로, 캐릭터 위치 재설정.
	void SwapIconColor(); // 아이콘 컬러 바꾸기.

	void SettingPartySlot(); // 파티 변경하는 UI 띄우기.
	void ResomeUI(); // 다시 맨처음 파티 UI 설정으로 돌아가기.
};
