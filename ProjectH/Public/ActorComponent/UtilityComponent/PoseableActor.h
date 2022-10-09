// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "GameFramework/Actor.h"
#include "PoseableActor.generated.h"


/*----------------

	PoseableMesh

-----------------*/

UCLASS()
class PROJECTH_API APoseableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APoseableActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetPoseMesh(USkeletalMeshComponent* Mesh);
	void SetCopyPose(USkeletalMeshComponent* Mesh);

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Poseable CoolTime")
		float FadeOutTime = 0.5;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component", meta = (AllowPrivateAccess = "true"))
		class UPoseableMeshComponent* PoseableMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material", meta = (AllowPrivateAccess = "true"))
		class UMaterialInstance* PoseableMaterial;

	UPROPERTY()
		class UMaterialInstanceDynamic* Materials;

	bool bSpawned = false;
	float FadeCnt;

};
