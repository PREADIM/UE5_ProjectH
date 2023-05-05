// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/JRPGCamera.h"
#include "Kismet/KismetMathLibrary.h"
#include "Tema/JRPG/JRPGPlayerController.h"

// Sets default values
AJRPGCamera::AJRPGCamera()
{
	PrimaryActorTick.bCanEverTick = true;


	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	RootComponent = Root;
	SpringArm->SetupAttachment(Root);
	Camera->SetupAttachment(SpringArm);

	CurrentSpringArm = SpringArm->TargetArmLength;
	TargetSpringArm = CurrentSpringArm;
}

void AJRPGCamera::BeginPlay()
{
	Super::BeginPlay();
	
}

void AJRPGCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurrentLocation = GetActorLocation();
	CurrentRotation = GetActorRotation();
	if (CurrentLocation != TargetLocation)
	{
		SetActorLocation(UKismetMathLibrary::VInterpTo(CurrentLocation, TargetLocation, DeltaTime, 5.0f));
	}

	if (CurrentRotation != TargetRotation)
	{
		SetActorRotation(UKismetMathLibrary::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 5.0f));
	}

	if (CurrentSpringArm != TargetSpringArm)
	{
		SpringArm->TargetArmLength = UKismetMathLibrary::FInterpTo(CurrentSpringArm, TargetSpringArm, DeltaTime, 3.0f);
	}

}

void AJRPGCamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}




