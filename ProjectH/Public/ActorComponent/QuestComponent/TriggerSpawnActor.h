// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "GameFramework/Actor.h"
#include "ActorComponent/QuestComponent/NPCInterface.h"
#include "TriggerSpawnActor.generated.h"

UCLASS()
class PROJECTH_API ATriggerSpawnActor : public AActor, public INPCInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATriggerSpawnActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString TriggerText;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class AProjectHCharacter* PlayerCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class ATriggerEventBase* OwnerTrigger; // �� ���ʹ� Ʈ���ſ� ���� ��ȯ�Ǵ� �����̹Ƿ�, 
	// �� ������ ������ �������ÿ� Trigger�� QuestNextStep()�� �������ش�.


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bBPBindFunc; // BPBindFunc�� ����Ѵٸ� �̰� True�� ���ش�.

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bCanThisInteract; // �� ���Ͱ� ���� ���ͷ�Ʈ �������� ����.

public:
	virtual void Interact_Implementation(class AProjectHCharacter* OwnerCharacter);

	// �ڡ� Trigger���� �����ȴٸ� ������ �ݵ�� ����
	UFUNCTION(BlueprintCallable)
		void SetTriggerBaseInit(class ATriggerEventBase* TriggerBase, AProjectHCharacter* OwnerCharacter);

	UFUNCTION(BlueprintImplementableEvent)
		void BPBindFunc();
};
