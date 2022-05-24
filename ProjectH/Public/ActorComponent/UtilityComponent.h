// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Components/ActorComponent.h"
#include "UtilityComponent.generated.h"



/*---------------------------

	Test Utility Component

----------------------------*/


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTH_API UUtilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UUtilityComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


public:
	/*-------------
		Poseable
	--------------*/
	void SetSK_Mesh(class USkeletalMeshComponent* Mesh); // �ڽ�Ƭ�� ����ɼ��� ������ �Լ��� �ƿ� �� ���´�.
	void SpawnPoseActor(FTransform Transform, class USkeletalMeshComponent* Mesh);



public:
	/*-------------
		Poseable
	--------------*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class APoseableActor> P_Actor;



private:
	/*-------------
		Poseable
	--------------*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PoseableMesh", meta = (AllowPrivateAccess = "true"))
		class APoseableActor* PoseableActor; // BP Pose������ �޽� ����� ����.
		
};
