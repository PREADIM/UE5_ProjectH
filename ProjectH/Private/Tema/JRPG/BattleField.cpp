// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/BattleField.h"

// Sets default values
ABattleField::ABattleField()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<class USceneComponent>(TEXT("RootComponent"));
	AllShotLocation = CreateDefaultSubobject<class USceneComponent>(TEXT("AllShotLocation"));
	MainStaticMesh = CreateDefaultSubobject<class UStaticMeshComponent>(TEXT("MainBody"));


	Unit1 = CreateDefaultSubobject<class USceneComponent>(TEXT("Unit1"));
	Unit2 = CreateDefaultSubobject<class USceneComponent>(TEXT("Unit2"));
	Unit3 = CreateDefaultSubobject<class USceneComponent>(TEXT("Unit3"));
	Enermy1 = CreateDefaultSubobject<class USceneComponent>(TEXT("Enermy1"));
	Enermy2 = CreateDefaultSubobject<class USceneComponent>(TEXT("Enermy2"));
	Enermy3 = CreateDefaultSubobject<class USceneComponent>(TEXT("Enermy3"));

	RootComponent = Root;
	MainStaticMesh->SetupAttachment(Root);
	AllShotLocation->SetupAttachment(Root);

	Unit1->SetupAttachment(Root);
	Unit2->SetupAttachment(Root);
	Unit3->SetupAttachment(Root);
	Enermy1->SetupAttachment(Root);
	Enermy2->SetupAttachment(Root);
	Enermy3->SetupAttachment(Root);


}

// Called when the game starts or when spawned
void ABattleField::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABattleField::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

