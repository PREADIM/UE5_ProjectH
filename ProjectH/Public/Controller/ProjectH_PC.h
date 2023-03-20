// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "PlayerControllerBase.h"
#include "Special/QTE/QTEWidgetPosition.h"
#include "Special/QTE/CameraTransition.h"
#include "Special/QTE/KeyEventInfo.h"
#include "ProjectH_PC.generated.h"

/**
 * 
 */




UCLASS()
class PROJECTH_API AProjectH_PC : public APlayerControllerBase
{
	GENERATED_BODY()

public:
	AProjectH_PC();

	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	virtual void SetupInputComponent() override;
	virtual void SetNewMouseSensitivity() override;

	void BeginInit();
	
	/* ---------------
		UI Open KEY 
	------------------*/
	void OpenQuestList();
	void OpenESC();
	void OpenOption();
	void MouseOnOff();


	/*---------------
		TEST QTE
	------------------*/

	void CreateQTE();
	// QTE를 실행시키는 함수.
	void StartQTEFunc(FKey Key, FQTEWidgetPosition ScreenPosition, float Time, float Dilation, FCameraTransition CameraTransition, FVector Location);
	// 타이머를 이용해서 해당 함수를 계속 실행하여, QTE를 성공했는지 실패했는지 검사.
	void QTETimerLoop(); 
	// 시간 초과, 성공, 키 실패를 처리할 함수.
	void EventCompleted(EQTEState State, FCameraTransition CameraTransition); 
	void ClearQTEWidget();

	//--------------------------------------------------------

	
	// 인터랙트와 퀘스트 콜리전을 껏다 켜서 다시한번 재 검사
	void SetInteractCollisionSetup();
	void SetQuestCollisionSetup();

	UFUNCTION()
		void PlayCinemiceMainUIHidden();
	UFUNCTION()
		void PlayCinemiceMainUIVisible();
	UFUNCTION(BlueprintCallable)
		void PlaySequenceProxy(int32 SequenceNumber); /* 게임 인스턴스의 PlaySequence 함수를 컨트롤러를 통해 실행해주는 함수 */

public:
	/*-----------------
		Public Values
	-------------------*/
	UPROPERTY(VisibleAnywhere)
		class UProjectHGameInstance* GI;

	UPROPERTY(VisibleAnywhere)
		class AProjectHCharacter* OwnerCharacter;
	
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


};
