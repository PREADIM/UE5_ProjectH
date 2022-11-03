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
		void EnermyCollisionOverlap(class AJRPGPlayerController* PC); // �÷��̾ ������

	UFUNCTION(BlueprintCallable)
		void PlayerCollisionOverlap(class AJRPGUnit* PlayerUnit); // ���� ������.

	void FieldEnermyDead(); // ���� ó�� �Լ�.

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 FieldEnermyNumber; // Save���Ͽ��� �̰ɷ� �˻��Ѵ�.

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<int32> EnermyUnits; // �� ���ָ� ����ϴ� ���, ���� ������������ �ִ��� �Ǵ�.

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bDead; // �� ��ü�� �׾����� ��Ҵ��� �����Ѵ�.

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class AJRPGGameMode* GM; // �� ��ü�� �׾����� ��Ҵ��� �����Ѵ�.

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAnimMontage* DeadMontage; 

};
