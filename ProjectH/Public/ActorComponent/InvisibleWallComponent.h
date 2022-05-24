// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Components/ActorComponent.h"
#include "InvisibleWallComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTH_API UInvisibleWallComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInvisibleWallComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


public:
	/*------------------
		Public Function
	--------------------*/

	// 3인칭 카메라와 캐릭터 사이에 시야를 가리는 메시를 찾는다.
	bool CameraTraceByWall(); 
	// CameraTraceByWall로 찾은 메쉬들을 DeltaTime으로 투명화한다.
	void StaticMeshInvisible(class AStaticMeshActor* StaticMesh, float SetOpacity);
	// 투명화 되어있는 Mesh들을 다시 원래대로 돌려놓는다.
	void ResetMaterial();

	void Reset(AStaticMeshActor* ResetWall);


	void SetOwnerCamera3P(class UCameraComponent* Camera3P) { OwnerCamera3P = Camera3P; }
	class UCameraComponent* GetOwnerCamera3P(UCameraComponent* Camera3P) { return OwnerCamera3P; }


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Invisible Material")
		class UMaterialInstance* InvisibleMaterial;

private:
	/* ----------------
		Private Value
	------------------*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* OwnerCamera3P;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class AProjectHCharacter* OwnerCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class AProjectH_PC* OwnerController;

	UPROPERTY()
		class UMaterialInstanceDynamic* IMaterial;


	/* 투명 벽 만들 때 사용하는 변수들. */
	FVector Camera3PStart;
	FVector Camera3PEnd;
	FCollisionObjectQueryParams CameraObjectParams;
	FCollisionQueryParams CameraQueryParams;


	
	//현재 투명으로 보이는 벽을 저장.
	TArray<class AStaticMeshActor*> HittedMesh;
	//TArray<class AStaticMeshActor> PrevMeshes;
	//class AStaticMeshActor* HittedMesh; // single LineTrace
	//TArray<class UMaterialInterface*> OriginalMaterials;
	TMap<class AStaticMeshActor*, TArray<class UMaterialInterface*>> OriginalMaterials;

	bool bMesh = false;

};
