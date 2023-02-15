// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PRojectH.h"
#include "GameFramework/Actor.h"
#include "MazeLight.generated.h"

UCLASS()
class PROJECTH_API AMazeLight : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMazeLight();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UBoxComponent* DynamicCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UPointLightComponent* Light;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UParticleSystemComponent* Effect;
	UPROPERTY(VisibleAnywhere)
		class UAudioComponent* Audio;

	UPROPERTY(EditAnywhere)
		bool bDontEffectVisible;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION()
		void OverlapCharacter(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OverlapEndCharacter(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void VisibleLight(bool bFlag);
};
