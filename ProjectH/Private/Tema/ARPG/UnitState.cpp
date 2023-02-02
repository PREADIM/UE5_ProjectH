#include "Tema/ARPG/UnitState.h"
#include "Tema/ARPG/ARPGUnitBase.h"


FUnitState::FUnitState()
{

}

void FUnitState::Init(AARPGUnitBase* Unit)
{
	OwnerUnit = Unit;

	HP = NormallyHP;
	MP = NormallyMP;
	AP = NormallyAP;
	ATK = NormallyATK;
	DEF = NormallyDEF;
	//Poise = NormallyPoise;
	//Poise는 노멀 모드 배틀모드에 강인도가 다르므로 배틀시작시에 적용
}

void FUnitState::SetTakeDamageHP(float TakeHP)
{
	HP = FMath::Clamp(TakeHP, 0.f, NormallyHP);
}

void FUnitState::SetAP(float TakeAP)
{
	float Temp = FMath::Clamp(TakeAP, 0.f, NormallyAP);
	AP = Temp;
	if (Temp == 0.0f)
	{
		if (OwnerUnit)
		{
			OwnerUnit->ZeroAP();
		}
	}
}


void FUnitState::ResetPoise()
{
	Poise = NormallyPoise;
}
