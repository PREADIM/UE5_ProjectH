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
