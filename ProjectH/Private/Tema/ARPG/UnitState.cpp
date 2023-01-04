#include "Tema/ARPG/UnitState.h"


FUnitState::FUnitState()
{

}

void FUnitState::SetTakeDamageHP(float TakeDamageHP)
{
	HP = TakeDamageHP;
}

void FUnitState::SetTakeDamageAP(float TakeDamageAP)
{
	AP = TakeDamageAP;
}
