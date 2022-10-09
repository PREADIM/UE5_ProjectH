// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/JRPGCamera.h"
#include "Kismet/KismetMathLibrary.h"
#include "Tema/JRPG/JRPGPlayerController.h"

// Sets default values
AJRPGCamera::AJRPGCamera()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
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

	if (CurrentSpringArm != TargetSpringArm)
	{
		SpringArm->TargetArmLength = UKismetMathLibrary::FInterpTo(CurrentSpringArm, TargetSpringArm, DeltaTime, 3.0f);
	}

}

// Called to bind functionality to input
void AJRPGCamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}




