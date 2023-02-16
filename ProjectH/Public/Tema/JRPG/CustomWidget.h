// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Blueprint/UserWidget.h"
#include "CustomWidget.generated.h"

/**
 * 
 */

UCLASS()
class PROJECTH_API UCustomWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere)
		class AJRPGPlayerController* OwnerController;

	UPROPERTY(VisibleAnywhere)
		class AJRPGGameMode* GM;

public:
	virtual void SetCloseFunction() {};
};


/*---------------------------------------------------------------------------

 �� �� ������ OwnerController�� LastWidget�� �ش��ϴ� Widget���� ���� �θ� Ŭ����

----------------------------------------------------------------------------*/