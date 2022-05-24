// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "TimelineNode.generated.h"




/*Ÿ�Ӷ��� ��� Ŭ���� �̴�� Ȱ���ص� ������ �׳� Ÿ�Ӷ����� �ʿ��� Ŭ������ FTimeline�� �����ص��ɵ� �ϴ�. */

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
		float TimelineLength; //Ÿ�Ӷ��� ����.

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Curve", meta = (AllowPrivateAccess = "true"))
		UCurveFloat* CurveF;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Curve", meta = (AllowPrivateAccess = "true"))
		UCurveVector* CurveV;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Curve", meta = (AllowPrivateAccess = "true"))
		UCurveLinearColor* CurveC;



	FTimeline LerpTimeline;
};
