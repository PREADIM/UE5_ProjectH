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
		FString InstigatorUnitName; // 가해자 유닛.
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
struct FCCState // CC기 상태를 별도로 가지고있음.
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bStun; // 애님 블프에서 사용.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		ECCType LastCCType;

	void SetCCType(ECCType CCType, bool bFlag);
};


UCLASS(Blueprintable, BlueprintType)
class PROJECTH_API UDebuffClass : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 DebuffTurn = 0; // 디버프가 유지 될 턴

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int32 CurrentDebuffTurn = 0; // 디버프가 유지 될 턴 Current

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EDebuffState DebuffState; // 디버프 상태

public:
	/* 디버프가 실행될 때 실행되는 함수 */
	UFUNCTION(BlueprintImplementableEvent)
		void DebuffFunction(class AJRPGUnit* Unit);

	/* 디버프가 끝날 때 실행되는 함수 */
	UFUNCTION(BlueprintImplementableEvent)
		void DebuffTurnEndFunction(class AJRPGUnit* Unit);

	/* 턴이 끝나면 턴 -1 카운트 */
	bool SetupCnt();

};
