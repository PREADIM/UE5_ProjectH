// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/ARPGPlayerController.h"
#include "Tema/ARPG/ARPGGameMode.h"
#include "Tema/ARPG/ARPGUnitBase.h"
#include "Tema/ARPG/Widget/ARPGWidgetMain.h"
#include "GameMode/ProjectHGameInstance.h"
#include "Tema/ARPG/Widget/ARPGWidget_BossHPView.h"
#include "Tema/ARPG/Widget/ARPGESCMenu.h"

AARPGPlayerController::AARPGPlayerController()
{
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
}

void AARPGPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	OwnerUnit = Cast<AARPGUnitBase>(InPawn);
	if (OwnerUnit == nullptr)
		return;

	GI = Cast<UProjectHGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (GI)
	{
		MouseSensitivity = GI->MS;
		SetNewMouseSensitivity();
	}

	GM = Cast<AARPGGameMode>(GetWorld()->GetAuthGameMode());
	if (GM && OwnerUnit)
		OwnerUnit->GM = GM;

	if (BP_WidgetMain)
	{
		WidgetMain = CreateWidget<UARPGWidgetMain>(GetWorld(), BP_WidgetMain);
		if (WidgetMain)
		{
			WidgetMain->Init(OwnerUnit);
			WidgetMain->AddToViewport();
		}
	}

	if (BP_ARPGESCMenu)
	{
		ARPGESCMenu = CreateWidget<UARPGESCMenu>(GetWorld(), BP_ARPGESCMenu);
		if(ARPGESCMenu)
		{
			ARPGESCMenu->OwnerController = this;
			ARPGESCMenu->Init(OwnerUnit);
		}
	}

	OnHiddenWidget.AddUFunction(this, FName("BindHiddenWidget"));
	OnVisibleWidget.AddUFunction(this, FName("BindVisibleWidget"));
}


void AARPGPlayerController::OnUnPossess()
{
	Super::OnUnPossess();
}

void AARPGPlayerController::SetNewMouseSensitivity()
{
	Super::SetNewMouseSensitivity();

	if (OwnerUnit)
		OwnerUnit->MouseSensitivity = MouseSensitivity;
}

void AARPGPlayerController::BeginPlay()
{
	Super::BeginPlay();
	SetPlaySound(NormalSound);
}

void AARPGPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction(TEXT("ESC"), IE_Released, this, &AARPGPlayerController::OpenESC);
}

void AARPGPlayerController::SetChargeAttacking(float Ratio)
{
	WidgetMain->SetChargeAttacking(Ratio);
}

void AARPGPlayerController::ChargeAttackInViewport(bool bFlag)
{
	WidgetMain->ChargeAttackInViewport(bFlag);
}



void AARPGPlayerController::SetBossHPWidget(bool bFlag, AARPGEnermy* Boss)
{
	if (bFlag)
	{
		if (BP_WidgetBossHP)
		{
			// ★★
			// 만일 보스가 여러마리인데 하나의 Boss HP가 먼저 존재했을경우.
			// 애초에 해당 HP 위젯을 세로 박스로 감싼후에 거기에 AddChild를 하는 형식.
			// 보스가 여러마리가 난입하는 경우에 해당 방식을 채택.
			
			WidgetBossHP = CreateWidget<UARPGWidget_BossHPView>(GetWorld(), BP_WidgetBossHP);
			if (WidgetBossHP)
			{
				WidgetBossHP->BossUnit = Boss;
				WidgetBossHP->Init(OwnerUnit);
				WidgetBossHP->AddToViewport();
			}
		}

	}
	else
	{
		if (WidgetBossHP)
		{
			WidgetBossHP->RemoveFromParent();
		}
	}
}


bool AARPGPlayerController::IsBossHPWidget()
{
	if (WidgetBossHP->IsInViewport())
	{
		return true;
	}
	else
		return false;
}

void AARPGPlayerController::SetPlaySound(USoundBase* Sound)
{
	if (!AudioComponent)
		return;

	if (!Sound)
	{
		StopSound();
		return;
	}
	else
		AudioComponent->SetSound(Sound);

	AudioComponent->Play();
}


void AARPGPlayerController::SetPlayNormalSound()
{
	if (!AudioComponent)
		return;

	AudioComponent->SetSound(NormalSound);
	AudioComponent->Play();
}

void AARPGPlayerController::StopSound()
{
	if (!AudioComponent)
		return;

	if (AudioComponent->IsPlaying())
		AudioComponent->Stop();
}


void AARPGPlayerController::BindVisibleWidget()
{
	if (WidgetMain)
	{
		if(WidgetMain->GetRenderOpacity() <= 0.1f)
			WidgetMain->SetRenderOpacity(1.0f);
	}

	if (WidgetBossHP)
	{
		if (WidgetBossHP->GetRenderOpacity() <= 0.1f)
			WidgetBossHP->SetRenderOpacity(1.0f);
	}
}


void AARPGPlayerController::BindHiddenWidget()
{
	if (WidgetMain)
	{
		if (WidgetMain->GetRenderOpacity() == 1.f)
			WidgetMain->SetRenderOpacity(0.f);
	}

	if (WidgetBossHP)
	{
		if (WidgetBossHP->GetRenderOpacity() == 1.f)
			WidgetBossHP->SetRenderOpacity(0.f);
	}
}


void AARPGPlayerController::OpenESC()
{
	ARPGESCMenu->ESCClick();
}
