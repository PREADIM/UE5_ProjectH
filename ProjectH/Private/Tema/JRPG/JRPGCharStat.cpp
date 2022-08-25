#include "Tema/JRPG/JRPGCharStat.h"


FJRPGCharStat::FJRPGCharStat()
{
	CharLevel = 1;
	Attack = 0;
	Shelid = 0.0f;
	MaxHP = 0.0f;
	MaxMP = 0.0f;
	MaxULT = 0.0f;
}

FJRPGCharStat::FJRPGCharStat(float HP, float MP, float AT, float DFS, float ULT)
{
	CharLevel = 1;
	Attack = AT;
	Shelid = DFS;
	MaxHP = HP;
	MaxMP = MP;
	MaxULT = ULT;
}