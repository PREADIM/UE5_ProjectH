// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/QuestComponent/TriggerSpawnActor.h"
#include "Character/ProjectHCharacter.h"
#include "ActorComponent/QuestComponent/TriggerEventBase.h"


// Sets default values
ATriggerSpawnActor::ATriggerSpawnActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void ATriggerSpawnActor::BeginPlay()
{
	Super::BeginPlay();
	
}


/* 플레이어가 인터랙트 하면 실행 */
void ATriggerSpawnActor::Interact_Implementation(class AProjectHCharacter* OwnerCharacter)
{
	/* 블루프린트 커스텀 */
	if(bBPBindFunc)
		BPBindFunc();
}

void ATriggerSpawnActor::SetTriggerBaseInit(ATriggerEventBase* TriggerBase, AProjectHCharacter* OwnerCharacter)
{
	OwnerTrigger = TriggerBase;
	PlayerCharacter = OwnerCharacter;
	if (PlayerCharacter)
		QuestComponent = PlayerCharacter->GetQuestComponent();
}

void ATriggerSpawnActor::ClearQuest()
{
	if (OwnerTrigger)
		OwnerTrigger->QuestNextStep();
}
