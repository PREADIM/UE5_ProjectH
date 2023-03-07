// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "UObject/NoExportTypes.h"
#include "DebuffClass.generated.h"
/**
 * 
 */

UENUM(BlueprintType)
enum class EDebuffState : uint8
{
	NONE UMETA(DispalyName = "NONE"),
	DEF_DOWN UMETA(DisplayName = "DEF_DOWN"),
	ATK_DOWN UMETA(DisplayName = "ATK_DOWN"),
	COLD UMETA(DisplayName = "COLD")
};


UCLASS(Blueprintable, BlueprintType)
class PROJECTH_API UDebuffClass : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 DebuffTurn; // 디버프가 유지 될 턴

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EDebuffState DebuffState; // 디버프 상태

public:
	UFUNCTION(BlueprintImplementableEvent)
		void DebuffFunction();
};
