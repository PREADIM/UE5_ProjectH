// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Blueprint/UserWidget.h"
#include "DropItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UDropItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* DropCharName;

	UPROPERTY()
		class AJRPGPlayerController* OwnerController;

public:
	void Init(FString CharName);

	UFUNCTION(BlueprintCallable)
		void MouseOff();
	
};
