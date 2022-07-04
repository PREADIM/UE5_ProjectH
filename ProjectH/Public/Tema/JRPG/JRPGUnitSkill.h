#pragma once
#include "ProjectH.h"
#include "Tema/JRPG/JRPGSkillState.h"
#include "JRPGUnitSkill.generated.h"


USTRUCT(BlueprintType)
struct FJRPGUnitSkill
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FJRPGSkillState NormalAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FJRPGSkillState Skill_1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FJRPGSkillState Skill_2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FJRPGSkillState Skill_3;
};

