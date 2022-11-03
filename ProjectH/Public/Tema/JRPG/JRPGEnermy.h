// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "GameFramework/Character.h"
#include "JRPGEnermy.generated.h"

UCLASS()
class PROJECTH_API AJRPGEnermy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AJRPGEnermy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
		void EnermyCollisionOverlap(class AJRPGPlayerController* PC); // 플레이어가 공격함

	UFUNCTION(BlueprintCallable)
		void PlayerCollisionOverlap(class AJRPGUnit* PlayerUnit); // 적이 공격함.

	void FieldEnermyDead(); // 죽음 처리 함수.

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 FieldEnermyNumber; // Save파일에서 이걸로 검색한다.

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<int32> EnermyUnits; // 적 유닛만 사용하는 기능, 무슨 유닛을가지고 있는지 판단.

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bDead; // 이 개체가 죽었는지 살았는지 결정한다.

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class AJRPGGameMode* GM; // 이 개체가 죽었는지 살았는지 결정한다.

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAnimMontage* DeadMontage; 

};
