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
	// QTE를 실행시키는 함수.
	void QTETimerLoop(); 
	// 타이머를 이용해서 해당 함수를 계속 실행하여, QTE를 성공했는지 실패했는지 검사.

	void EventCompleted(EQTEState State, FCameraTransition CameraTransition); 
	// 시간 초과, 성공, 키 실패를 처리할 함수.
	void ClearQTEWidget();


	// 마우스 감도 새로운 값 적용하는 함수.
	void SetNewMouseSensitivity();

	// 인터랙트와 퀘스트 콜리전을 껏다 켜서 다시한번 재 검사
	void SetInteractCollisionSetup();
	void SetQuestCollisionSetup();

	UFUNCTION()
		void PlayCinemiceMainUIHidden();
	UFUNCTION()
		void PlayCinemiceMainUIVisible();

public:
	UPROPERTY(VisibleAnywhere)
		class AProjectHCharacter* OwnerCharacter;

	FOnVisibleWidget OnVisibleWidget; // 시네마틱 연출할때 위젯 켜기.
	FOnHiddenWidget OnHiddenWidget; // 시네마틱 연출할때 끄기.

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
