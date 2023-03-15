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

	void FieldEnermyDead(); // ���� ó�� �Լ�.
	void DeadUnit();
	void ReturnToField();
	void BattleStart();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bBPBindFunc;
	UFUNCTION(BlueprintImplementableEvent)
		void BPBindFunc_DeadUnit(); // �������� ���� ������ �Լ�.

public:
	UPROPERTY(VisibleAnywhere)
		FTransform DefaultTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 FieldEnermyNumber; // Save���Ͽ��� �̰ɷ� �˻��Ѵ�.

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FEnermys> EnermyUnits; // �� ���ָ� ����ϴ� ���, ���� ������������ �ִ��� �Ǵ�.

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
