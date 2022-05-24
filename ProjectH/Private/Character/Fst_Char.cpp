// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Fst_Char.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Engine/StaticMeshActor.h"
#include "ActorComponent/InvisibleWallComponent.h"
#include "ActorComponent/UtilityComponent.h"
#include "Kismet/KismetMathLibrary.h"


/*----------------

		Init

------------------*/


AFst_Char::AFst_Char()
{
	PrimaryActorTick.bCanEverTick = true; // Tick을 사용한다.
	//PrimaryActorTick.SetTickFunctionEnable() true또는 false로 틱을 껏다 키기. 최적화.


}




/*----------------

	Move & Utils

-----------------*/

void AFst_Char::LMB()
{
}


void AFst_Char::RMB()
{

}



void AFst_Char::TestKey()
{
	/*int32 i = 0;
	FString path = FPaths::SourceConfigDir(); // 소스파일이있는 곳의 config파일.
	path += "DefaultMainGameSetting.ini";
	GConfig->SetInt(TEXT("/Script/GameSetting.MainGameSetting"), TEXT("num"), 1000, path); //수정
	GConfig->GetInt(TEXT("/Script/GameSetting.MainGameSetting"), TEXT("num"), i, path); // 가져오기.
	//ini파일은 첫 시작때만 적용되지만, 이렇게 실시간으로도 가져올수 있다.

	SCREEN_DEBUG(3.0f, FColor::Yellow, "Config %s", *path);*/

	//SetViewChange();

}



void AFst_Char::PoseableTestKey()
{
	UtilityComponent->SpawnPoseActor(GetMesh()->GetComponentToWorld(), GetMesh());
}


/*-------------------

	Virtual Function

---------------------*/

void AFst_Char::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("LMB", IE_Pressed, this, &AFst_Char::LMB);
	PlayerInputComponent->BindAction("RMB", IE_Pressed, this, &AFst_Char::RMB);

	PlayerInputComponent->BindAction("TestKey", IE_Pressed, this, &AFst_Char::TestKey);
	PlayerInputComponent->BindAction("PoseableTestKey", IE_Pressed, this, &AFst_Char::PoseableTestKey);
}

void AFst_Char::BeginPlay()
{
	Super::BeginPlay();

	/*auto Setting = GetDefault<UMainGameSetting>();
	if (Setting)
	{
		test = Setting->GetTemp();
		num = Setting->GetNum();
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, FString::Printf(TEXT("Main Setting Config TEST : %d"), test));
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("Main Setting Config NUM : %d"), num));
		//AddOnScreenDebugMessage는 첫 인자가 -1이면 밀면서 갱신, 아니면 고정하여 갱신이다.
	}*/

}


void AFst_Char::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}



void AFst_Char::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AFst_Char::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
}



/*----------------------

	Interface Function

-----------------------*/
void AFst_Char::OverlapedEvent_Implementation()
{
	SCREEN_DEBUG(3.f, FColor::Green, "INTERFACE");
}
