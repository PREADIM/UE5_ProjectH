// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/JRPGCamera.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AJRPGCamera::AJRPGCamera()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AJRPGCamera::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
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

}

// Called to bind functionality to input
void AJRPGCamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

