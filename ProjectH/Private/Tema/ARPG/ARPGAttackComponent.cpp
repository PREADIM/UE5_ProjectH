// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/ARPG/ARPGAttackComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Tema/ARPG/ARPGEnermy.h"

UARPGAttackComponent::UARPGAttackComponent()
{

}

void UARPGAttackComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UARPGAttackComponent::Init(TArray<TSubclassOf<UAttackClass>> BP_Attacks)
{
	OwnerUnit = Cast<AARPGUnitBase>(GetOwner());
	if (OwnerUnit == nullptr)
		return;

	AttackCnt = 0;
	Attacks.Empty();
	MaxAttackDistance = -100.0f;

	for (TSubclassOf<UAttackClass> BP_Attack : BP_Attacks)
	{
		UAttackClass* temp = NewObject<UAttackClass>(this, BP_Attack); //보류
		if (temp)
		{
			temp->Init();
			temp->OwnerUnit = OwnerUnit;
			Attacks.Emplace(temp);
			AttackCnt++;
			MaxAttackDistance = MaxAttackDistance < temp->AttackStruct.AttackDistance ? temp->AttackStruct.AttackDistance : MaxAttackDistance;
			// 공격의 가장 최대길이를 가져와서 해당 길이로 무빙을 칠건지 안칠건지 알수있음

		}
	}
}

UAttackClass* UARPGAttackComponent::GetAttackClass(int32 index)
{
	if (Attacks.IsValidIndex(index))
	{
		return Attacks[index];
	}
	return nullptr;
}


