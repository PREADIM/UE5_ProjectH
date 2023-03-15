// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Tema/JRPG/JRPGUnitBase.h"
#include "Tema/JRPG/JRPGDropStruct.h"
#include "JRPGEnermy.generated.h"



UCLASS()
class PROJECTH_API AJRPGFieldEnermy : public AJRPGUnitBase
{
	GENERATED_BODY()

public:
	AJRPGFieldEnermy();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void FieldEnermyDead(); // 죽음 처리 함수.
	void DeadUnit();
	void ReturnToField();
	void BattleStart();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bBPBindFunc;
	UFUNCTION(BlueprintImplementableEvent)
		void BPBindFunc_DeadUnit(); // 블프에서 따로 제작한 함수.

public:
	UPROPERTY(VisibleAnywhere)
		FTransform DefaultTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 FieldEnermyNumber; // Save파일에서 이걸로 검색한다.

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FEnermys> EnermyUnits; // 적 유닛만 사용하는 기능, 무슨 유닛을가지고 있는지 판단.

	UPROPERTY(VisibleAnywhere)
		bool bDead;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class AJRPGGameMode* GM;

	UPROPERTY(VisibleAnywhere)
		class AJRPGFieldAIController* OwnerAIController;

	UPROPERTY(EditAnywhere)
		class UBehaviorTree* BT;
	class UBehaviorTree* GetBT();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAnimMontage* DeadMontage; 

	UPROPERTY(VisibleAnywhere)
		FJRPGDropStruct DropStruct;

};
