// Fill out your copyright notice in the Description page of Project Settings.


#include "Special/QTE/QTECamera.h"
#include "Controller/ProjectH_PC.h"

// Sets default values
AQTECamera::AQTECamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	LocationComponent = CreateDefaultSubobject<class USceneComponent>(TEXT("LocationComponent"));
	Camera = CreateDefaultSubobject<class UCameraComponent>(TEXT("Camera"));
	RootComponent = LocationComponent;
	Camera->SetupAttachment(LocationComponent);


}

// Called when the game starts or when spawned
void AQTECamera::BeginPlay()
{
	Super::BeginPlay();

	CT.NewCamera = this;
	CT.BlendTime = BlendTime;
}



void AQTECamera::QTE_Implementation(class AProjectH_PC* OwnerController)
{
	if (!OwnerController)
		return;

	OwnerController->StartQTEFunc(Key, ScreenPosition, TotalTime, Dilation, CT, LocationComponent->GetComponentLocation());
}


