// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/Maze/MazeLight.h"
#include "Kismet/GameplayStatics.h"
#include "Tema/ARPG/ARPGUnit.h"

// Sets default values
AMazeLight::AMazeLight()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Effect = CreateDefaultSubobject<class UParticleSystemComponent>(TEXT("Effect"));
	Light = CreateDefaultSubobject<class UPointLightComponent>(TEXT("Light"));	
	DynamicCollision = CreateDefaultSubobject<class UBoxComponent>(TEXT("Collision"));
	Audio = CreateDefaultSubobject<class UAudioComponent>(TEXT("Audio"));

	RootComponent = Effect;
	Light->SetupAttachment(RootComponent);
	DynamicCollision->SetupAttachment(RootComponent);
	Audio->SetupAttachment(RootComponent);

	Light->SetVisibility(false);
	Light->SetCastShadows(true);
	Effect->SetVisibility(false);

	DynamicCollision->SetBoxExtent(FVector(700.f, 700.f, 500.f));
	DynamicCollision->OnComponentBeginOverlap.AddDynamic(this, &AMazeLight::OverlapCharacter);
	DynamicCollision->OnComponentEndOverlap.AddDynamic(this, &AMazeLight::OverlapEndCharacter);

}

// Called when the game starts or when spawned
void AMazeLight::BeginPlay()
{
	Super::BeginPlay();

}

void AMazeLight::OverlapCharacter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AARPGUnit* Unit = Cast<AARPGUnit>(OtherActor);
	if (Unit)
	{
		VisibleLight(true);
		//_DEBUG("Overlap Light");
	}

}

void AMazeLight::OverlapEndCharacter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AARPGUnit* Unit = Cast<AARPGUnit>(OtherActor);
	if (Unit)
	{
		VisibleLight(false);
		//_DEBUG("Overlap End Light");
	}

}

void AMazeLight::VisibleLight(bool bFlag)
{
	if (bFlag)
	{
		Light->SetVisibility(true);
		Effect->SetVisibility(true);
		if (Audio)
		{
			Audio->Play();
			_DEBUG("Play");
		}
	}
	else
	{
		Light->SetVisibility(false);
		Effect->SetVisibility(false);
		if (Audio)
		{
			Audio->Stop();
			_DEBUG("Stop");
		}
	}
}


