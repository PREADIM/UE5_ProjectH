// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/Widget/ARPGWidget_Charge.h"
#include "Components/ProgressBar.h"

void UARPGWidget_Charge::Init(AARPGUnitBase* Unit)
{
	Super::Init(Unit);
	SetRenderOpacity(0.f);
}

void UARPGWidget_Charge::NativeConstruct()
{
	Super::NativeConstruct();

}

void UARPGWidget_Charge::BindCharge(float Ratio)
{
	Charge->SetPercent(Ratio);
}
