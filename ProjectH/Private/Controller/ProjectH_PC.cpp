// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/ProjectH_PC.h"
#include "UI/MainQuestUI.h"
#include "UI/QTEMainUI.h"
#include "UI/QTESlotUI.h"
#include "UI/ESCMenu.h"
#include "Character/ProjectHCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameMode/ProjectHGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Special/QTE/QTECamera.h"
#include "UI/QuestMainCanvasWidget.h"
#include "UI/InteractWidget.h"


AProjectH_PC::AProjectH_PC()
{

}

void AProjectH_PC::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	OnVisibleWidget.AddUFunction(this, FName("PlayCinemiceMainUIVisible"));
	OnHiddenWidget.AddUFunction(this, FName("PlayCinemiceMainUIHidden"));

}


void AProjectH_PC::BeginPlay()
{
	Super::BeginPlay();
}

void AProjectH_PC::BeginInit()
{
	GI = Cast<UProjectHGameInstance>(UGameplayStatics::GetGameInstance(this));
	MouseSensitivity = GI->MS;
	GI->PlaySequence(1, this);	
	
	if (BP_MainQuestUI && BP_MainQuestIconWidget && OwnerCharacter)
	{
		MainQuestUI = CreateWidget<UMainQuestUI>(GetWorld(), BP_MainQuestUI);
		MainQuestUI->OwnerCharacter = OwnerCharacter;
		MainQuestUI->OwnerController = this;
		MainQuestUI->Init();
		MainQuestUI->AddToViewport();

		MainQuestIconWidget = CreateWidget<UQuestMainCanvasWidget>(GetWorld(), BP_MainQuestIconWidget);
		MainQuestIconWidget->OwnerController = this;
		MainQuestIconWidget->AddToViewport();
	}
	
	SetNewMouseSensitivity();
	MainQuestUI->InteractWidget->InteractButton->OnClicked.AddDynamic(OwnerCharacter, &AProjectHCharacter::InteractKey);

}


void AProjectH_PC::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	OwnerCharacter = Cast<AProjectHCharacter>(InPawn);	
	if (OwnerCharacter)
	{
		PossessActor = OwnerCharacter;
		BeginInit();
	}
		
}

void AProjectH_PC::OnUnPossess()
{
	Super::OnUnPossess();
}

void AProjectH_PC::OpenQuestList()
{
	IsOpen = MainQuestUI->OpenUI(IsOpen); // �Ű������� ���� ���� Not ���� ��������.
	//���⼭ â Ű�� ���� �־ �ɵ�.
}


/* ---------------
	UI Open KEY
------------------*/

void AProjectH_PC::PlayCinemiceMainUIHidden()
{
	MainQuestUI->SetRenderOpacity(0.f);
	MainQuestIconWidget->SetRenderOpacity(0.f);
}

void AProjectH_PC::PlayCinemiceMainUIVisible()
{
	MainQuestUI->SetRenderOpacity(1.f);
	MainQuestIconWidget->SetRenderOpacity(1.f);
}

void AProjectH_PC::PlaySequenceProxy(int32 SequenceNumber)
{
	if (GI)
		GI->PlaySequence(SequenceNumber, this);
}

class UCanvasPanelSlot* AProjectH_PC::AddChildCanvas(class UUserWidget* Widget)
{
	if (MainQuestIconWidget)
		return MainQuestIconWidget->AddChildCanvas(Widget);

	return nullptr;
}

void AProjectH_PC::MainQuestIconWidgetSetup(UCanvasPanelSlot* CanvasSlot, FVector Location)
{
	if (MainQuestIconWidget)
		MainQuestIconWidget->MainQuestIconWidgetSetup(CanvasSlot, Location);
}

UQuestComponent* AProjectH_PC::GetQuestComponent()
{
	if (OwnerCharacter)
		return OwnerCharacter->GetQuestComponent();
	else
		return nullptr;
}

/* �ٸ� �ʿ��� �Ϸ��� ���� ���η��� ����Ʈ���� �Ϸ� ��Ű�� */
void AProjectH_PC::FromOtherMapNextQuestStep(int32 NextStepNumber)
{
	if (OwnerCharacter)
		OwnerCharacter->FromOtherMapNextQuestStep(NextStepNumber);
}

void AProjectH_PC::OpenESC()
{
	MainQuestUI->OpenESCMenu();
}

void AProjectH_PC::OpenOption()
{
	MainQuestUI->SettingKey();
}


void AProjectH_PC::MouseOnOff()
{
	if (bShowMouseCursor)
	{
		SetShowMouseCursor(false);
		SetInputMode(FInputModeGameOnly());
	}
	else
	{
		int32 x, y;
		GetViewportSize(x, y);
		SetMouseLocation(UKismetMathLibrary::FTrunc(x / 2), UKismetMathLibrary::FTrunc(y / 2));
		SetShowMouseCursor(true);
		SetInputMode(FInputModeGameAndUI());
	}
}



void AProjectH_PC::CreateQTE()
{
	if (BP_QTEMainUI && OwnerCharacter)
	{
		QTEMain = CreateWidget<UQTEMainUI>(GetWorld(), BP_QTEMainUI);
		SetInputMode(FInputModeGameOnly());
		QTEMain->AddToViewport();
	}
}


/* QTE �̺�Ʈ�� ���� �ٽ� �Լ�. �ش� �Լ����� EQTEState�� ����� ���� �� ������ �ݺ��Ѵ�. */
/* �ش� �Լ��� �����ؾ� QTE�� ���� �۵� �ȴ�. Ʈ���Ÿ� ����ص�����, ī�޶� ���͸� ������ �ڿ� �����ص� �� ���ϴ�. */
void AProjectH_PC::StartQTEFunc(FKey Key, FQTEWidgetPosition ScreenPosition, float Time, float Dilation, FCameraTransition CameraTransition, FVector Location)
{
	CurrentCameraTransition = CameraTransition;

	if (CurrentCameraTransition.NewCamera)
		SetViewTargetWithBlend(Cast<AActor>(CurrentCameraTransition.NewCamera), CurrentCameraTransition.BlendTime);

	if (OwnerCharacter)
	{
		GetWorldSettings()->SetTimeDilation(Dilation);
		OwnerCharacter->bQTE = true;
	}

	CurrentEventInfo.SetValues(EQTEState::Pending, Key, ScreenPosition, Time); // ���� �̺�Ʈ �ʱ�ȭ.

	if (QTEMain)
	{
		CurrentQTEWidget = QTEMain->AddQTEWidget(Key, Time, ScreenPosition); // QTE ���� ���� �� �ؽ�Ʈ�� �ð� �ʱ�ȭ.
		SetControlRotation(UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(), Location));
		// �ü� ����
		GetWorld()->GetTimerManager().SetTimer(QTETimerHandle, this, &AProjectH_PC::QTETimerLoop, EventRate, true);
	}
}

// QTE �˻� ���� �Լ�.
void AProjectH_PC::QTETimerLoop()
{
	float ExpiredTime = CurrentEventInfo.ExpiredTime + EventRate + GetWorld()->GetDeltaSeconds();
	CurrentEventInfo.ExpiredTime = ExpiredTime;

	if (CurrentQTEWidget)
		CurrentQTEWidget->UpdateTime(ExpiredTime);


	if (CurrentEventInfo.State == EQTEState::Success) // ����
	{
		_DEBUG("Success");
		GetWorld()->GetTimerManager().ClearTimer(QTETimerHandle);
		EventCompleted(EQTEState::Success, CurrentCameraTransition);
	}


	if (CurrentEventInfo.State == EQTEState::FailKey) // Ű �� ������
	{
		_DEBUG("Fail Key");
		GetWorld()->GetTimerManager().ClearTimer(QTETimerHandle);
		EventCompleted(EQTEState::FailKey, CurrentCameraTransition);
	}
		

	if (CurrentEventInfo.TotalTime <= ExpiredTime) // �ð� �ʰ� 
	{
		_DEBUG("Fail Time");
		GetWorld()->GetTimerManager().ClearTimer(QTETimerHandle);
		CurrentEventInfo.State = EQTEState::FailTime;
		EventCompleted(EQTEState::FailTime, CurrentCameraTransition);
	}

}

/* QTE ���� �� ���� */
void AProjectH_PC::EventCompleted(EQTEState State, FCameraTransition CameraTransition)
{
	if (OwnerCharacter)
	{
		GetWorldSettings()->SetTimeDilation(1.0f); // ���� �ð� ���󺹱�
		OwnerCharacter->bQTE = false;
	}
	
	if (CameraTransition.NewCamera)
		SetViewTargetWithBlend(OwnerCharacter, CameraTransition.BlendTime);

	float EndTime = CurrentQTEWidget->PlayWidgetAnim(State == EQTEState::Success); // Success�� ���� �ִϸ��̼�, �ƴϸ� ���� �ִϸ��̼�

	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, this, &AProjectH_PC::ClearQTEWidget, EndTime, false);

}

// QTE �ʱ�ȭ.
void AProjectH_PC::ClearQTEWidget()
{
	CurrentQTEWidget->RemoveFromParent();
	CurrentQTEWidget = nullptr;
	QTEMain->QTEWidget = nullptr;
}



void AProjectH_PC::SetNewMouseSensitivity()
{
	Super::SetNewMouseSensitivity();

	if (OwnerCharacter)
		OwnerCharacter->MouseSensitivity = MouseSensitivity;
}

void AProjectH_PC::SetInteractCollisionSetup()
{
	OwnerCharacter->InteractCollisionSetUp();
}

void AProjectH_PC::SetQuestCollisionSetup()
{
	OwnerCharacter->QuestCollisionSetUp();
}



void AProjectH_PC::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction(TEXT("OpenList"), IE_Released, this, &AProjectH_PC::OpenQuestList);
	InputComponent->BindAction(TEXT("ESC"), IE_Released, this, &AProjectH_PC::OpenESC);
	// �ӽ÷� PŰ�� ������.
	InputComponent->BindAction(TEXT("Option"), IE_Released, this, &AProjectH_PC::OpenOption);
	InputComponent->BindAction(TEXT("MouseOnOff"), IE_Released, this, &AProjectH_PC::MouseOnOff);


}

