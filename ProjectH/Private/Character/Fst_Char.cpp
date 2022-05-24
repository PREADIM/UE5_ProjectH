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
	PrimaryActorTick.bCanEverTick = true; // Tick�� ����Ѵ�.
	//PrimaryActorTick.SetTickFunctionEnable() true�Ǵ� false�� ƽ�� ���� Ű��. ����ȭ.


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
	FString path = FPaths::SourceConfigDir(); // �ҽ��������ִ� ���� config����.
	path += "DefaultMainGameSetting.ini";
	GConfig->SetInt(TEXT("/Script/GameSetting.MainGameSetting"), TEXT("num"), 1000, path); //����
	GConfig->GetInt(TEXT("/Script/GameSetting.MainGameSetting"), TEXT("num"), i, path); // ��������.
	//ini������ ù ���۶��� ���������, �̷��� �ǽð����ε� �����ü� �ִ�.

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
		//AddOnScreenDebugMessage�� ù ���ڰ� -1�̸� �и鼭 ����, �ƴϸ� �����Ͽ� �����̴�.
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
