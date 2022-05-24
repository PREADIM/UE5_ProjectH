// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "GameFramework/Actor.h"
#include "TrafficActor.generated.h"

UCLASS()
class PROJECTH_API ATrafficActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrafficActor();


	/*------------------
		Traffic Mesh
	--------------------*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TrafficMesh)
		class UStaticMeshComponent* TrafficMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TrafficMesh)
		class UStaticMeshComponent* LEDMesh_Red;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TrafficMesh)
		class UStaticMeshComponent* LEDMesh_Green;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TrafficMesh)
		class UStaticMeshComponent* LEDMesh_Number_Right;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TrafficMesh)
		class UStaticMeshComponent* LEDMesh_Number_Left;


	/*-----------------
		Point Light
	-------------------*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Light)
		class UPointLightComponent* RedLight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Light)
		class UPointLightComponent* GreenLight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Light)
		class UPointLightComponent*	NumberLight;

	/*-------------
		Material
	---------------*/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = LED)
		class UMaterialInstance* LED_NoneMaterial;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = LED)
		class UMaterialInstance* LED_RedMaterial;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = LED)
		class UMaterialInstance* LED_GreenMaterial;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = LED)
		class UMaterialInstance* LED_NumberMaterial;


	/*---------------------
		Dynamic Material
	----------------------*/

	UPROPERTY()
		class UMaterialInstanceDynamic* MI_None;
	UPROPERTY()
		class UMaterialInstanceDynamic* MI_Red;
	UPROPERTY()
		class UMaterialInstanceDynamic* MI_Green;
	UPROPERTY()
		class UMaterialInstanceDynamic* MI_Number_Left;
	UPROPERTY()
		class UMaterialInstanceDynamic* MI_Number_Right;

	/*-----------
		Timer
	------------*/

	FTimerHandle RedTimer;
	FTimerHandle GreenTimer;
	FTimerHandle NumberCount;
	FTimerHandle GreenOnOff;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TimerTime)
		float RedTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TimerTime)
		float GreenTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TimerTime)
		float NumberTime;


	bool bGreenOn = false;
	float Cnt;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	void TrafficStart();
	void Init(); // √ ±‚»≠
	void RedLED(bool bFlag);
	void GreenLED(bool bFlag);
	void SetNumber(bool bFlaf, int32 Count);

	void StartGreen();
	void StartCount();
	void GreenFlash();
};
