// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Blueprint/UserWidget.h"
#include "ARPGWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UARPGWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void Init(class AARPGUnitBase* Unit);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class AARPGUnitBase* OwnerUnit;

};
