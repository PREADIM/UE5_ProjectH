// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "UObject/NoExportTypes.h"
#include "DebuffClass.generated.h"
/**
 * 
 */


USTRUCT(BlueprintType)
struct FDebuffStruct
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY()
		FString InstigatorUnitName; // ������ ����.
	UPROPERTY()
		class UDebuffClass* DebuffClass = nullptr;

	FDebuffStruct() : InstigatorUnitName(""), DebuffClass(nullptr) {}
	FDebuffStruct(FString AttackUnitName, class UDebuffClass* AttackDebuff);

	bool operator==(const FDebuffStruct& Other) const
	{
		return InstigatorUnitName.Equals(Other.InstigatorUnitName);
	}
};

uint32 GetTypeHash(const FDebuffStruct& DebuffStruct);

UENUM(BlueprintType)
enum class EDebuffState : uint8
{
	NONE UMETA(DispalyName = "NONE"),
	DEF_DOWN UMETA(DisplayName = "DEF_DOWN"),
	ATK_DOWN UMETA(DisplayName = "ATK_DOWN"),
	STUN UMETA(DisplayName = "STUN")
};


UENUM(BlueprintType)
enum class ECCType : uint8
{
	NONE UMETA(DisplayName = "NONE"),
	STUN UMETA(DisplayName = "STUN")
};


USTRUCT(BlueprintType)
struct FCCState // CC�� ���¸� ������ ����������.
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bStun; // �ִ� �������� ���.
};


UCLASS(Blueprintable, BlueprintType)
class PROJECTH_API UDebuffClass : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 DebuffTurn = 0; // ������� ���� �� ��

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int32 CurrentDebuffTurn = 0; // ������� ���� �� �� Current

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EDebuffState DebuffState; // ����� ����

public:
	UFUNCTION(BlueprintImplementableEvent)
		void DebuffFunction(class AJRPGUnit* Unit);

	UFUNCTION(BlueprintImplementableEvent)
		void DebuffTurnEndFunction(class AJRPGUnit* Unit);

	bool SetupCnt();

};
