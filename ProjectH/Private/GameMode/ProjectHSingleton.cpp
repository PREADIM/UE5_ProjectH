// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/ProjectHSingleton.h"



UProjectHSingleton::UProjectHSingleton()
{

}


UProjectHSingleton* UProjectHSingleton::GetSingleton()
{
	if (GEngine)
	{
		static UProjectHSingleton* ST = Cast<UProjectHSingleton>(GEngine->GameSingleton);
		return ST;
	}
	return nullptr;
}