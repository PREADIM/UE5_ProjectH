// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "UObject/NoExportTypes.h"
#include "QuestingFunction.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class PROJECTH_API UQuestingFunction : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Flag1Text;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Flag2Text;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Flag3Text;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class AQuestNPCBase* OwnerNPC;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class APlayerControllerBase* OwnerController;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UProjectHGameInstance* GI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 FlagCnt; // 플래그의 갯수 1~3

public:
	UFUNCTION(BlueprintImplementableEvent)
		void Flag1Function();

	UFUNCTION(BlueprintImplementableEvent)
		void Flag2Function();

	UFUNCTION(BlueprintImplementableEvent)
		void Flag3Function();

};
