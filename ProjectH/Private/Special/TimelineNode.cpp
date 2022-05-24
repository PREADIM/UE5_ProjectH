// Fill out your copyright notice in the Description page of Project Settings.


#include "Special/TimelineNode.h"
#include "Components/TimelineComponent.h"

// Sets default values
ATimelineNode::ATimelineNode()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CurveF = nullptr;
	CurveF = nullptr;
	CurveC = nullptr;
	TimelineLength = 0.5f;
}

// Called when the game starts or when spawned
void ATimelineNode::BeginPlay()
{
	Super::BeginPlay();

	if (CurveF && CurveV && CurveC)
	{
		FOnTimelineFloat CurveF_Callback;
		FOnTimelineVector CurveV_Callback;
		FOnTimelineLinearColor CurveC_Callback;
		// ������ Ŀ�꿡 ����� ���̳��� ��������Ʈ �ݹ� �Լ�.
		// ���̳��� ��������Ʈ �̱⶧���� �����ϴ� �Լ����� �ݵ�� UFUNCTION()�� ȣ��Ǿ��־����.

		FOnTimelineEventStatic TimelineFinished_Callback;
		//Ÿ�Ӷ����� Finished �Լ�

		CurveF_Callback.BindUFunction(this, FName("�Լ��̸�1"));
		CurveV_Callback.BindUFunction(this, FName("�Լ��̸�2"));
		CurveC_Callback.BindUFunction(this, FName("�Լ��̸�3"));
		TimelineFinished_Callback.BindUFunction(this, FName("�Լ� �̸�4"));
		// �� ��������Ʈ�� �Լ� ���ε�.

		LerpTimeline.AddInterpFloat(CurveF, CurveF_Callback);
		LerpTimeline.AddInterpVector(CurveV, CurveV_Callback);
		LerpTimeline.AddInterpLinearColor(CurveC, CurveC_Callback);
		LerpTimeline.SetTimelineFinishedFunc(TimelineFinished_Callback);
		//FTimeline ����ü�� ���� Ŀ��� �ش� Ŀ�꿡 �ش�� ��������Ʈ �¾�. Finished �Լ��� �¾�.

		LerpTimeline.SetTimelineLength(TimelineLength);
		//Ÿ�Ӷ��� ���� �¾�
	}

	LerpTimeline.PlayFromStart();	
}

// Called every frame
void ATimelineNode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	LerpTimeline.TickTimeline(DeltaTime);
	// Deltatime ����

}

