// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/JRPGComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Tema/JRPG/JRPGGameMode.h"
#include "Tema/JRPG/JRPGPlayerController.h"
#include "Tema/JRPG/MainUI/JRPGTemaUI.h"


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
	
}


// Called every frame
void UJRPGComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UJRPGComponent::Init()
{
	// OnwerController���� �ش�ĳ������ ��ų�� ������ �ʱ�ȭ�Ѵ�.
	//�ش� �Լ��� ȣ��ɶ��� ��Ʈ�ѷ������� ������ ��Ʈ�ѷ� �ȿ��ִ� JRPG ������ �����ϰ� ������ ������.
	// OwnerUnit�� �̿��ؼ� ���� ���� �ʱ�ȭ. OwnerUnit�� Current HP�� MP ����.
}

void UJRPGComponent::BattleStart()
{
	if (OwnerController && OwnerUnit)
	{

	}
}


void UJRPGComponent::BattleEnd()
{
	//Ư�� �ൿ�� �������� �ش� �Լ��� ����.
	if (GM)
	{
		GM->TurnEnd();
	}
}


void UJRPGComponent::SetOwnerPCAndGM(class AJRPGPlayerController* PC , class AJRPGGameMode* GameMode)
{
	OwnerController = PC;
	GM = GameMode;
	if (OwnerController)
	{
		BattleWidget = OwnerController->TemaMainUI->BattleWidget;

	}
}