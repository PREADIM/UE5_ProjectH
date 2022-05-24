// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "GameFramework/Actor.h"
#include "Special/QTE/CameraTransition.h"
#include "Special/QTE/QTEWidgetPosition.h"
#include "Interface/QTEInterface.h"
#include "QTECamera.generated.h"

UCLASS()
class PROJECTH_API AQTECamera : public AActor, public IQTEInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AQTECamera();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
		class USceneComponent* LocationComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
		class UCameraComponent* Camera;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QTE)
		float BlendTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QTE)
		FKey Key;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QTE)
		FQTEWidgetPosition ScreenPosition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QTE)
		float TotalTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QTE)
		float Dilation;

	UPROPERTY(VisibleAnywhere)
		FCameraTransition CT;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void QTE(class AProjectH_PC* OwnerController);
	virtual void QTE_Implementation(class AProjectH_PC* OwnerController); // QTE ½ÇÇà

};
