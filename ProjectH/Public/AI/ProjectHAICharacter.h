// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "GameFramework/Character.h"
#include "ProjectHAICharacter.generated.h"


/*----------------------

	Base AI Character

------------------------*/

UCLASS()
class PROJECTH_API AProjectHAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AProjectHAICharacter();

protected:
	virtual void BeginPlay();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime);
	virtual void PossessedBy(AController* NewController);


public:
	class UBehaviorTree* GetBT() { return BT; }


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UBehaviorTree* BT; // 커스텀할수 있게 DummyBT를 설정해두었음.


};
