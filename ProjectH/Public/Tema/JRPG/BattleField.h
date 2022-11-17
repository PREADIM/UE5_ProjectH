// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "GameFramework/Actor.h"
#include "BattleField.generated.h"

UCLASS()
class PROJECTH_API ABattleField : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABattleField();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
		class USceneComponent* Root;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
		class USceneComponent* AllShotLocation; // 필드의 전체가 보이는 카메라 구도 위치.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
		class UStaticMeshComponent* MainStaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
		class USceneComponent* Unit1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
		class USceneComponent* Unit2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
		class USceneComponent* Unit3;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
		class USceneComponent* Enermy1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
		class USceneComponent* Enermy2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
		class USceneComponent* Enermy3;

};
