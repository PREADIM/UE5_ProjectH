// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "TimelineNode.generated.h"




/*타임라인 노드 클래스 이대로 활용해도 되지만 그냥 타임라인이 필요한 클래스에 FTimeline만 적용해도될듯 하다. */

UCLASS()
class PROJECTH_API ATimelineNode : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATimelineNode();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Curve")
		float TimelineLength; //타임라인 길이.

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Curve", meta = (AllowPrivateAccess = "true"))
		UCurveFloat* CurveF;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Curve", meta = (AllowPrivateAccess = "true"))
		UCurveVector* CurveV;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Curve", meta = (AllowPrivateAccess = "true"))
		UCurveLinearColor* CurveC;



	FTimeline LerpTimeline;
};
