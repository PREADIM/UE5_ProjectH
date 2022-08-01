// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Blueprint/UserWidget.h"
#include "JRPGSettingHoriList.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UJRPGSettingHoriList : public UUserWidget
{

	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		TSubclassOf<class UJRPGSettingPartyIcon> BP_UnitIcon; // 아이콘들

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UHorizontalBox* HoriList;

	UPROPERTY(VisibleAnywhere)
		class AJRPGPlayerController* OwnerController;

	UPROPERTY(VisibleAnywhere)
		TArray<int32> CharNums; // 최대 4칸의 CharNum들

public:
	void Init();

};
