// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/MainUI/PartySettingField.h"

// Sets default values
APartySettingField::APartySettingField()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	Unit1 = CreateDefaultSubobject<USceneComponent>(TEXT("Unit1"));
	Unit2 = CreateDefaultSubobject<USceneComponent>(TEXT("Unit2"));
	Unit3 = CreateDefaultSubobject<USceneComponent>(TEXT("Unit3"));
	Unit4 = CreateDefaultSubobject<USceneComponent>(TEXT("Unit4"));

	Unit1_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Unit1_Camera"));
	Unit2_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Unit2_Camera"));
	Unit3_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Unit3_Camera"));
	Unit4_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Unit4_Camera"));


	RootComponent = Root;
	Camera->SetupAttachment(Root);

	Unit1->SetupAttachment(Root);
	Unit2->SetupAttachment(Root);
	Unit3->SetupAttachment(Root);
	Unit4->SetupAttachment(Root);

	Unit1_Camera->SetupAttachment(Unit1);
	Unit2_Camera->SetupAttachment(Unit2);
	Unit3_Camera->SetupAttachment(Unit3);
	Unit4_Camera->SetupAttachment(Unit4);

	Unit1_Camera->SetActive(false);
	Unit2_Camera->SetActive(false);
	Unit3_Camera->SetActive(false);
	Unit4_Camera->SetActive(false);
}

// Called when the game starts or when spawned
void APartySettingField::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APartySettingField::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APartySettingField::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

