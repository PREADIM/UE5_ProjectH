// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/UtilityComponent.h"
#include "ActorComponent/UtilityComponent/PoseableActor.h"

// Sets default values for this component's properties
UUtilityComponent::UUtilityComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UUtilityComponent::BeginPlay()
{
	Super::BeginPlay();	

}


// Called every frame
void UUtilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UUtilityComponent::SetSK_Mesh(class USkeletalMeshComponent* Mesh)
{
	if (P_Actor != nullptr)
	{
		PoseableActor = Cast<APoseableActor>(P_Actor->GetDefaultObject());
		PoseableActor->SetPoseMesh(Mesh); // 포즈메쉬 설정.
	}
	else
	{
		SCREEN_DEBUG(5.0f, FColor::Blue, "SET MESH FAIL");
	}
}



void UUtilityComponent::SpawnPoseActor(FTransform Transform, USkeletalMeshComponent* Mesh)
{
	if (P_Actor != nullptr)
	{
		APoseableActor* PoseActors = GetWorld()->SpawnActor<APoseableActor>(P_Actor, Transform);
		PoseActors->SetCopyPose(Mesh);
	}
	
}