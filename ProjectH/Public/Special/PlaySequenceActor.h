// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "GameFramework/Actor.h"
#include "PlaySequenceActor.generated.h"

UCLASS()
class PROJECTH_API APlaySequenceActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlaySequenceActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UProjectHGameInstance* GI;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class APlayerControllerBase* PCBase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class ULevelSequence* Sequence;

	UFUNCTION(BlueprintImplementableEvent)
		void BPBindFunc(float EndTime);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UFUNCTION()
		void VisibleWidget();


};
