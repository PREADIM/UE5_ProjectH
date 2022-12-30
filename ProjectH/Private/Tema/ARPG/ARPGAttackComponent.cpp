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
	OwnerEnermy = Cast<AARPGEnermy>(GetOwner());
	if (OwnerEnermy == nullptr)
	{
		_DEBUG("AttackComp Not OwnerEnermy");
		return;
	}

	AttackCnt = 0;
	Attacks.Empty();
	MaxAttackDistance = -100.0f;

	for (TSubclassOf<UAttackClass> BP_Attack : BP_Attacks)
	{
		UAttackClass* temp = Cast<UAttackClass>(BP_Attack->GetDefaultObject());
		if (temp)
		{
			temp->Init();
			Attacks.Add(temp);	
			_DEBUG("Attack distance : %f", Attacks[AttackCnt]->AttackStruct.AttackDistance);
			AttackCnt++;
			MaxAttackDistance = MaxAttackDistance < temp->AttackStruct.AttackDistance ? temp->AttackStruct.AttackDistance : MaxAttackDistance;
			// 공격의 가장 최대길이를 가져와서 해당 길이로 무빙을 칠건지 안칠건지 알수있음

		}
	}
}


void UARPGAttackComponent::SetAttackDelegate(int32 index, FName FuncName)
{
	if (Attacks.IsValidIndex(index))
	{
		if (OwnerEnermy)
		{
			Attacks[index]->CppAttackFunc.AddUFunction(OwnerEnermy, FuncName);
			_DEBUG("Cpp AddUFunction %s", *FuncName.ToString());
		}
	}
}

UAttackClass* UARPGAttackComponent::GetAttackClass(int32 index)
{
	if (Attacks.IsValidIndex(index))
	{
		return Attacks[index];
	}

	_DEBUG("Not Dynamic Delegate Error");
	return nullptr;
}


