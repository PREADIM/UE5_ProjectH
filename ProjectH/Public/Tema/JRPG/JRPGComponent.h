// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Components/ActorComponent.h"
#include "JRPGComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTH_API UJRPGComponent : public UActorComponent
{
	GENERATED_BODY()

// ������Ʈ�� ��Ʋ ���� -> ���Ӹ�忡�� �޾ƿ�. �ش� ������ ��ų�� ���������� ������ �ʱ�ȭ.
// �� �� ������Ʈ�� �ش� ĳ������ ������ �÷��̾� ��Ʈ�ѷ��� ������ �������ֱ⶧����, �ش� ������ �̿��ؼ�
//    ���� ����. �� �����̳� ��ų�� ���� ����, �ش� ĳ���͸��� ������ �ְ� �Ѵ�.(���� �Լ��� ����.)
// �� ���� �� ��ų�� ���� �� ����ÿ� ���Ӹ���� �� ���Ḧ �����ϰ��Ѵ�.


public:	
	// Sets default values for this component's properties
	UJRPGComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


public:
	void Init(); // �÷��̾� ��Ʈ�ѷ��� �����Ͽ� ���� �ʱ�ȭ.
	void BattleStart();
	void BattleEnd();
	void SetOwnerPCAndGM(class AJRPGPlayerController* PC, class AJRPGGameMode* GameMode);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class AJRPGPlayerController* OwnerController;

	UPROPERTY(VisibleAnywhere)
		class AJRPGGameMode* GM;
		
};
