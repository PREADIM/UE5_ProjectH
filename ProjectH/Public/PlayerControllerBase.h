// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "GameFramework/PlayerController.h"
#include "PlayerControllerBase.generated.h"

/**
 * 
 */

DECLARE_MULTICAST_DELEGATE(FOnHiddenWidget);
DECLARE_MULTICAST_DELEGATE(FOnVisibleWidget);

UCLASS()
class PROJECTH_API APlayerControllerBase : public APlayerController
{
	GENERATED_BODY()

public:
	
	FOnVisibleWidget OnVisibleWidget; // 시네마틱 연출할때 위젯 켜기.
	FOnHiddenWidget OnHiddenWidget; // 시네마틱 연출할때 끄기.	

	UPROPERTY(VisibleAnywhere)
		float MouseSensitivity;

	UPROPERTY()
		class AActor* PossessActor;

public:
	// 마우스 감도 새로운 값 적용하는 함수.
	virtual void SetNewMouseSensitivity();

	UFUNCTION(BlueprintCallable)
		void MouseOn();
	UFUNCTION(BlueprintCallable)
		void MouseOff();

	/* 입력 제한 함수 */
	void DisableInputActor(bool bFlag);
};
