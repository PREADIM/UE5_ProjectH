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

	//--------------------------------------------------------

	
	// ���ͷ�Ʈ�� ����Ʈ �ݸ����� ���� �Ѽ� �ٽ��ѹ� �� �˻�
	void SetInteractCollisionSetup();
	void SetQuestCollisionSetup();

	UFUNCTION()
		void PlayCinemiceMainUIHidden();
	UFUNCTION()
		void PlayCinemiceMainUIVisible();
	UFUNCTION(BlueprintCallable)
		void PlaySequenceProxy(int32 SequenceNumber); /* ���� �ν��Ͻ��� PlaySequence �Լ��� ��Ʈ�ѷ��� ���� �������ִ� �Լ� */

	/* ������ ������ �׽� MainWidget�� �����󿡼� �߰� �����. */
	class UCanvasPanelSlot* AddChildCanvas(class UUserWidget* Widget);
	void MainQuestIconWidgetSetup(class UCanvasPanelSlot* CanvasSlot, FVector Location);

	/* ------------------
		QuestComponent
	---------------------*/

	class UQuestComponent* GetQuestComponent();

	/* �ٸ� �������� �Ϸ��� ����Ʈ�� �����Ѵ�. */
	void FromOtherMapNextQuestStep(int32 NextStepNumber);

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
		TSubclassOf<class UQuestMainCanvasWidget> BP_MainQuestIconWidget;
	UPROPERTY()
		class UQuestMainCanvasWidget* MainQuestIconWidget;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
		TSubclassOf<class UQuestInfo> BP_QuestInfoUI;
	UPROPERTY()
		class UQuestInfo* QuestInfoUI;



	/*---------------
		TEST QTE
	------------------*/

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


	/*---------------
		TEST QTE
	------------------*/

	void CreateQTE();
	// QTE�� �����Ű�� �Լ�.
	void StartQTEFunc(FKey Key, FQTEWidgetPosition ScreenPosition, float Time, float Dilation, FCameraTransition CameraTransition, FVector Location);
	// Ÿ�̸Ӹ� �̿��ؼ� �ش� �Լ��� ��� �����Ͽ�, QTE�� �����ߴ��� �����ߴ��� �˻�.
	void QTETimerLoop();
	// �ð� �ʰ�, ����, Ű ���и� ó���� �Լ�.
	void EventCompleted(EQTEState State, FCameraTransition CameraTransition);
	void ClearQTEWidget();
};
