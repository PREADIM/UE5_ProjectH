// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "GameFramework/PlayerController.h"
#include "Special/QTE/QTEWidgetPosition.h"
#include "Special/QTE/CameraTransition.h"
#include "Special/QTE/KeyEventInfo.h"
#include "ProjectH_PC.generated.h"

/**
 * 
 */

DECLARE_MULTICAST_DELEGATE(FOnHiddenWidget);
DECLARE_MULTICAST_DELEGATE(FOnVisibleWidget);


UCLASS()
class PROJECTH_API AProjectH_PC : public APlayerController
{
	GENERATED_BODY()

public:
	/*-------------------
		Public Function
	---------------------*/
	AProjectH_PC();


	virtual void BeginPlay();
	virtual void OnPossess(APawn* InPawn);
	virtual void OnUnPossess();
	virtual void SetupInputComponent();


	void BeginInit();
	

	/* ---------------
		UI Open KEY 
	------------------*/
	void OpenQuestList();
	void OpenESC();
	void OpenOption();
	void MouseOnOff();

	void CreateQTE();
	void StartQTEFunc(FKey Key, FQTEWidgetPosition ScreenPosition, float Time, float Dilation, FCameraTransition CameraTransition, FVector Location);
	// QTE�� �����Ű�� �Լ�.
	void QTETimerLoop(); 
	// Ÿ�̸Ӹ� �̿��ؼ� �ش� �Լ��� ��� �����Ͽ�, QTE�� �����ߴ��� �����ߴ��� �˻�.

	void EventCompleted(EQTEState State, FCameraTransition CameraTransition); 
	// �ð� �ʰ�, ����, Ű ���и� ó���� �Լ�.
	void ClearQTEWidget();


	// ���콺 ���� ���ο� �� �����ϴ� �Լ�.
	void SetNewMouseSensitivity();

	// ���ͷ�Ʈ�� ����Ʈ �ݸ����� ���� �Ѽ� �ٽ��ѹ� �� �˻�
	void SetInteractCollisionSetup();
	void SetQuestCollisionSetup();

	UFUNCTION()
		void PlayCinemiceMainUIHidden();
	UFUNCTION()
		void PlayCinemiceMainUIVisible();

public:
	UPROPERTY(VisibleAnywhere)
		class AProjectHCharacter* OwnerCharacter;

	FOnVisibleWidget OnVisibleWidget; // �ó׸�ƽ �����Ҷ� ���� �ѱ�.
	FOnHiddenWidget OnHiddenWidget; // �ó׸�ƽ �����Ҷ� ����.

public:
	/*-----------------
		Public Values
	-------------------*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
		TSubclassOf<class UMainQuestUI> BP_MainQuestUI;
	UPROPERTY()
		class UMainQuestUI* MainQuestUI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
		TSubclassOf<class UQuestInfo> BP_QuestInfoUI;
	UPROPERTY()
		class UQuestInfo* QuestInfoUI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
		TSubclassOf<class UQTEMainUI> BP_QTEMainUI;
	UPROPERTY()
		class UQTEMainUI* QTEMain;


	UPROPERTY()
		class UQTESlotUI* CurrentQTEWidget;
	UPROPERTY(VisibleAnywhere)
		FKeyEventInfo CurrentEventInfo;

	UPROPERTY(VisibleAnywhere)
		FCameraTransition CurrentCameraTransition;
	float EventRate = 0.05f;

	FTimerHandle QTETimerHandle;

	UPROPERTY(VisibleAnywhere)
		float MouseSensitivity;

	bool IsOpen = false;


};
