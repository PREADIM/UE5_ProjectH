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
		// 지정한 커브에 사용할 다이나믹 델리게이트 콜백 함수.
		// 다이나믹 델리게이트 이기때문에 지정하는 함수들은 반드시 UFUNCTION()이 호출되어있어야함.

		FOnTimelineEventStatic TimelineFinished_Callback;
		//타임라인의 Finished 함수

		CurveF_Callback.BindUFunction(this, FName("함수이름1"));
		CurveV_Callback.BindUFunction(this, FName("함수이름2"));
		CurveC_Callback.BindUFunction(this, FName("함수이름3"));
		TimelineFinished_Callback.BindUFunction(this, FName("함수 이름4"));
		// 각 델리게이트에 함수 바인드.

		LerpTimeline.AddInterpFloat(CurveF, CurveF_Callback);
		LerpTimeline.AddInterpVector(CurveV, CurveV_Callback);
		LerpTimeline.AddInterpLinearColor(CurveC, CurveC_Callback);
		LerpTimeline.SetTimelineFinishedFunc(TimelineFinished_Callback);
		//FTimeline 구조체에 각종 커브와 해당 커브에 해당된 델리게이트 셋업. Finished 함수도 셋업.

		LerpTimeline.SetTimelineLength(TimelineLength);
		//타임라인 길이 셋업
	}

	LerpTimeline.PlayFromStart();	
}

// Called every frame
void ATimelineNode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	LerpTimeline.TickTimeline(DeltaTime);
	// Deltatime 전달

}

