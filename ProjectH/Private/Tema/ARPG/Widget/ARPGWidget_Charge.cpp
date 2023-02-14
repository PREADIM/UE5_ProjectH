// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/Widget/ARPGWidget_Charge.h"
#include "Components/ProgressBar.h"
#include "Tema/ARPG/ARPGUnitBase.h"
#include "Kismet/KismetMathLibrary.h"

void UARPGWidget_Charge::Init(AARPGUnitBase* Unit)
{
	Super::Init(Unit);

	SetRenderOpacity(0.f);
}

void UARPGWidget_Charge::NativeConstruct()
{
	Super::NativeConstruct();

	if(OwnerUnit)
		OwnerUnit->OnChargeAttackInit.AddUFunction(this, FName("SetCurrentColor"));
}

void UARPGWidget_Charge::BindCharge(float Ratio)
{
	if (bChargeHalf)
	{
		CurrentColor = UKismetMathLibrary::CInterpTo(CurrentColor, ChargeColor, GetWorld()->GetDeltaSeconds(), 3.f);
		//Charge->GetWidgetStyle().FillImage.TintColor = FSlateColor(CurrentColor);
		Charge->SetFillColorAndOpacity(CurrentColor);
	}
	else
	{
		if (Ratio >= 0.5f)
			bChargeHalf = true;
	}

	Charge->SetPercent(Ratio);
}


void UARPGWidget_Charge::SetCurrentColor()
{
	CurrentColor = NormalColor;
	Charge->SetFillColorAndOpacity(NormalColor);
	Charge->SetPercent(0.f);
	bChargeHalf = false;
}