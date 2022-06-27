// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Character/ProjectHCharacter.h"
#include "QuestStruct.h"
#include "Fst_Char.generated.h"



/*------------------------
	First Main Character
-------------------------*/

/**
 * 
 */
UCLASS()
class PROJECTH_API AFst_Char : public AProjectHCharacter
{
	GENERATED_BODY()

public:



private:
	/* ----------------
		Private Value
	------------------*/


public:
	/* ----------------
		Public Value
	------------------*/


public:
	/*----------------
			Init
	------------------*/
	AFst_Char();

public:
	/*----------------
		Move & Utils
	-----------------*/

	virtual void LMB() override;
	virtual void RMB() override;
	void TestKey();
	void PoseableTestKey();


public:
	/*-------------------
		Virtual Function
	---------------------*/
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;



	/*---------------------
		Interface Function
	----------------------*/

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "OverlapedEvent")
		void OverlapedEvent();
	virtual void OverlapedEvent_Implementation() override;
	
	
};
