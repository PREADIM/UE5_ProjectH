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
	IsOpen = MainQuestUI->OpenUI(IsOpen); // 매개변수로 보낸 것의 Not 값을 돌려받음.
	//여기서 창 키는 사운드 넣어도 될듯.
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

/* 다른 맵에서 완료한 것을 메인레벨 퀘스트에서 완료 시키기 */
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


/* QTE 이벤트를 위한 핵심 함수. 해당 함수에서 EQTEState의 결과를 도출 할 때까지 반복한다. */
/* 해당 함수를 실행해야 QTE가 정상 작동 된다. 트리거를 사용해도좋고, 카메라 액터를 생성한 뒤에 실행해도 될 듯하다. */
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

	CurrentEventInfo.SetValues(EQTEState::Pending, Key, ScreenPosition, Time); // 현재 이벤트 초기화.

	if (QTEMain)
	{
		CurrentQTEWidget = QTEMain->AddQTEWidget(Key, Time, ScreenPosition); // QTE 슬롯 생성 및 텍스트와 시간 초기화.
		SetControlRotation(UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(), Location));
		// 시선 고정
		GetWorld()->GetTimerManager().SetTimer(QTETimerHandle, this, &AProjectH_PC::QTETimerLoop, EventRate, true);
	}
}

// QTE 검사 루프 함수.
void AProjectH_PC::QTETimerLoop()
{
	float ExpiredTime = CurrentEventInfo.ExpiredTime + EventRate + GetWorld()->GetDeltaSeconds();
	CurrentEventInfo.ExpiredTime = ExpiredTime;

	if (CurrentQTEWidget)
		CurrentQTEWidget->UpdateTime(ExpiredTime);


	if (CurrentEventInfo.State == EQTEState::Success) // 성공
	{
		_DEBUG("Success");
		GetWorld()->GetTimerManager().ClearTimer(QTETimerHandle);
		EventCompleted(EQTEState::Success, CurrentCameraTransition);
	}


	if (CurrentEventInfo.State == EQTEState::FailKey) // 키 잘 못누름
	{
		_DEBUG("Fail Key");
		GetWorld()->GetTimerManager().ClearTimer(QTETimerHandle);
		EventCompleted(EQTEState::FailKey, CurrentCameraTransition);
	}
		

	if (CurrentEventInfo.TotalTime <= ExpiredTime) // 시간 초과 
	{
		_DEBUG("Fail Time");
		GetWorld()->GetTimerManager().ClearTimer(QTETimerHandle);
		CurrentEventInfo.State = EQTEState::FailTime;
		EventCompleted(EQTEState::FailTime, CurrentCameraTransition);
	}

}

/* QTE 성공 및 실패 */
void AProjectH_PC::EventCompleted(EQTEState State, FCameraTransition CameraTransition)
{
	if (OwnerCharacter)
	{
		GetWorldSettings()->SetTimeDilation(1.0f); // 지연 시간 원상복구
		OwnerCharacter->bQTE = false;
	}
	
	if (CameraTransition.NewCamera)
		SetViewTargetWithBlend(OwnerCharacter, CameraTransition.BlendTime);

	float EndTime = CurrentQTEWidget->PlayWidgetAnim(State == EQTEState::Success); // Success면 성공 애니메이션, 아니면 실패 애니메이션

	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, this, &AProjectH_PC::ClearQTEWidget, EndTime, false);

}

// QTE 초기화.
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
	// 임시로 P키에 지정함.
	InputComponent->BindAction(TEXT("Option"), IE_Released, this, &AProjectH_PC::OpenOption);
	InputComponent->BindAction(TEXT("MouseOnOff"), IE_Released, this, &AProjectH_PC::MouseOnOff);


}

