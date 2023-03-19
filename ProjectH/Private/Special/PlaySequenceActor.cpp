// Fill out your copyright notice in the Description page of Project Settings.


#include "Special/PlaySequenceActor.h"
#include "PlayerControllerBase.h"
#include <LevelSequencePlayer.h>
#include <LevelSequenceActor.h>

// Sets default values
APlaySequenceActor::APlaySequenceActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void APlaySequenceActor::BeginPlay()
{
	Super::BeginPlay();

	ALevelSequenceActor* LQActor;
	float EndTime = 5.0f;
	if (Sequence)
	{
		ULevelSequencePlayer* Player = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), Sequence, FMovieSceneSequencePlaybackSettings(), LQActor);
		if (Player)
		{
			Player->Play();
			EndTime = Player->GetEndTime().AsSeconds();
		}

		if (PCBase)
		{
			PCBase->OnHiddenWidget.Broadcast();
			FTimerHandle Handle;
			GetWorld()->GetTimerManager().SetTimer(Handle, this, &APlaySequenceActor::VisibleWidget, EndTime, false);
		}

	}

	BPBindFunc(EndTime);
}



void APlaySequenceActor::VisibleWidget()
{
	if (PCBase)
		PCBase->OnVisibleWidget.Broadcast();
}
