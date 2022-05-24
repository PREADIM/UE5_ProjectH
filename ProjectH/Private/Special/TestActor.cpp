// Fill out your copyright notice in the Description page of Project Settings.


#include "Special/TestActor.h"

// Sets default values
ATestActor::ATestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RedLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("RED_LIGHT"));
	GreenLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("Green_LIGHT"));
	NumberLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("Number_LIGHT"));
}

// Called when the game starts or when spawned
void ATestActor::BeginPlay()
{
	Super::BeginPlay();

	RedLED(true);
	GreenLED(true);

	//FOnTimelineFloat TLFloat;
	//TLFloat.BindUFunction(this, FName("TestFloat"));
	//TL.AddInterpFloat(CurveF, TLFloat);
	//TL.SetTimelineLength(0.5f);	
	//TL.PlayFromStart();
}

// Called every frame
void ATestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//TL.TickTimeline(DeltaTime);

}


void ATestActor::TestFloat(float num)
{
	FRotator Rotator = FMath::Lerp(FRotator(0.f, 0.f, 0.f), FRotator(0.f, 90.f, 0.f), num);
	SetActorRotation(Rotator);
	_DEBUG("%f", num);
}


void ATestActor::RedLED(bool bFlag)
{
	if (bFlag)
		_DEBUG("RED");

	RedLight->SetVisibility(true);
}

void ATestActor::GreenLED(bool bFlag)
{
	if (bFlag)
		_DEBUG("Green");
	GreenLight->SetVisibility(true);
}