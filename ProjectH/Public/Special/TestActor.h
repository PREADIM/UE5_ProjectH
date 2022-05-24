// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "TestActor.generated.h"

UCLASS()
class PROJECTH_API ATestActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Light)
		class UPointLightComponent* RedLight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Light)
		class UPointLightComponent* GreenLight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Light)
		class UPointLightComponent* NumberLight;

	FTimerHandle Handle;


	bool bflag = false;

	FTimeline TL;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Curve")
		UCurveFloat* CurveF;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void TestFloat(float num);

	void RedLED(bool bFlag);
	void GreenLED(bool bFlag);

};
