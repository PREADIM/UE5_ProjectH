// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/QuestComponent/TriggerSpawnActor.h"

// Sets default values
ATriggerSpawnActor::ATriggerSpawnActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATriggerSpawnActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATriggerSpawnActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ATriggerSpawnActor::Interact_Implementation(class AProjectHCharacter* OwnerCharacter)
{
	if(bBPBindFunc)
		BPBindFunc();
}

void ATriggerSpawnActor::SetTriggerBaseInit(ATriggerEventBase* TriggerBase, AProjectHCharacter* OwnerCharacter)
{
	OwnerTrigger = TriggerBase;
	PlayerCharacter = OwnerCharacter;
}
