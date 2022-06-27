// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Components/ActorComponent.h"
#include "JRPGComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTH_API UJRPGComponent : public UActorComponent
{
	GENERATED_BODY()

// 컴포넌트의 배틀 시작 -> 게임모드에서 받아옴. 해당 유닛의 스킬및 공격정보를 위젯에 초기화.
// ㄴ 각 컴포넌트는 해당 캐릭터의 정보와 플레이어 컨트롤러의 정보를 가지고있기때문에, 해당 정보를 이용해서
//    공격 구현. 각 공격이나 스킬에 대한 것은, 해당 캐릭터마다 가지고 있게 한다.(가상 함수로 만듬.)
// ㄴ 공격 및 스킬을 쓰고 턴 종료시에 게임모드의 턴 종료를 실행하게한다.


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
	void Init(); // 플레이어 컨트롤러에 접근하여 위젯 초기화.
	void BattleStart();
	void BattleEnd();
	void SetOwnerPCAndGM(class AJRPGPlayerController* PC, class AJRPGGameMode* GameMode);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class AJRPGPlayerController* OwnerController;

	UPROPERTY(VisibleAnywhere)
		class AJRPGGameMode* GM;
		
};
