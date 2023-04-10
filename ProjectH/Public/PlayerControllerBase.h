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
	
	FOnVisibleWidget OnVisibleWidget; // �ó׸�ƽ �����Ҷ� ���� �ѱ�.
	FOnHiddenWidget OnHiddenWidget; // �ó׸�ƽ �����Ҷ� ����.	

	UPROPERTY(VisibleAnywhere)
		float MouseSensitivity;

	UPROPERTY()
		class AActor* PossessActor;

public:
	// ���콺 ���� ���ο� �� �����ϴ� �Լ�.
	virtual void SetNewMouseSensitivity();

	UFUNCTION(BlueprintCallable)
		void MouseOn();
	UFUNCTION(BlueprintCallable)
		void MouseOff();

	/* �Է� ���� �Լ� */
	void DisableInputActor(bool bFlag);
};
