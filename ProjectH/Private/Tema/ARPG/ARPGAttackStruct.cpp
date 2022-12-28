#include "Tema/ARPG/ARPGAttackStruct.h"



FARPGAttackStruct::FARPGAttackStruct()
{
	bCanThisAttack = true;
	CurrentCnt = 0;
}

FARPGAttackStruct::FARPGAttackStruct(float Distance)
{
	bCanThisAttack = true;
	AttackDistance = Distance;
	CurrentCnt = 0;
}

