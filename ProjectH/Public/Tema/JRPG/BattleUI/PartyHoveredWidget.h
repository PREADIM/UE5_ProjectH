// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PartyHoveredWidget.generated.h"

/**
 * 
 */
//UCLASS() 일단 아래로 해본다.
UCLASS(Meta = (DisableNativeTick))
class PROJECTH_API UPartyHoveredWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* CharName;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* CharLevel;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* CharHP;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* CharMP;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* CharAttack;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* CharShelid;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* MaxULT;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* UnitPriority;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* CurrentExp;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* NextExp;

	
	int32 CharNum;

	UPROPERTY()
		class AJRPGPlayerController* OwnerController;


public:
	void Init();
	void SetText(FString Text);

};
