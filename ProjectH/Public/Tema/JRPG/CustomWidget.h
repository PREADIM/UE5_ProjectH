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

 ★ 이 위젯은 OwnerController의 LastWidget에 해당하는 Widget들을 위한 부모 클래스

----------------------------------------------------------------------------*/