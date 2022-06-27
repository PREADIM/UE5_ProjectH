// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/JRPGComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Tema/JRPG/JRPGGameMode.h"


// Sets default values for this component's properties
UJRPGComponent::UJRPGComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UJRPGComponent::BeginPlay()
{
	Super::BeginPlay();
	// GM = Cast<AJRPGGameMode>(UGameplayStatics::GetGameMode(GetWorld())); 보류



}


// Called every frame
void UJRPGComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UJRPGComponent::Init()
{
	//해당 함수가 호출될때는 컨트롤러정보가 있으니 컨트롤러 안에있는 JRPG 위젯을 생성하고 정보를 셋팅함.
}

void UJRPGComponent::BattleStart()
{
	if (OwnerController)
	{

	}
}


void UJRPGComponent::BattleEnd()
{
	//특정 행동이 끝났으면 해당 함수를 실행.
	if (GM)
	{
		GM->TurnEnd();

	}

}


void UJRPGComponent::SetOwnerPCAndGM(class AJRPGPlayerController* PC , class AJRPGGameMode* GameMode)
{
	OwnerController = PC;
	GM = GameMode;
}