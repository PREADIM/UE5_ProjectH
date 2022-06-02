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

	bool IsOpen = false;
private:
	UPROPERTY(VisibleAnywhere)
		class AProjectHCharacter* OwnerCharacter;

	
};
