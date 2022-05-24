// Fill out your copyright notice in the Description page of Project Settings.


#include "Special/TrafficActor.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Components/PointLightComponent.h"


// Sets default values
ATrafficActor::ATrafficActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	TrafficMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TrafficMesh"));
	LEDMesh_Red = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LED_RED"));
	LEDMesh_Green = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LED_GREEN"));
	LEDMesh_Number_Right = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LED_RIGHT"));
	LEDMesh_Number_Left = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LED_LEFT"));

	RedLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("RED_LIGHT"));
	GreenLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("Green_LIGHT"));
	NumberLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("Number_LIGHT"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Traffic(TEXT("StaticMesh'/Game/PROJECT/BP_CLASS/Asset/03_StaticMesh/ShinHo/TrafficMesh.TrafficMesh'"));
	if (Traffic.Succeeded())
		TrafficMesh->SetStaticMesh(Traffic.Object);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> LED_Mesh(TEXT("StaticMesh'/Game/PROJECT/BP_CLASS/Asset/03_StaticMesh/ShinHo/SinHo_LED.SinHo_LED'"));
	if (LED_Mesh.Succeeded())
	{
		LEDMesh_Red->SetStaticMesh(LED_Mesh.Object);
		LEDMesh_Green->SetStaticMesh(LED_Mesh.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> LED_Number(TEXT("StaticMesh'/Game/PROJECT/BP_CLASS/Asset/03_StaticMesh/ShinHo/SinHo_NumberLED.SinHo_NumberLED'"));
	if (LED_Number.Succeeded())
	{
		LEDMesh_Number_Left->SetStaticMesh(LED_Number.Object);
		LEDMesh_Number_Right->SetStaticMesh(LED_Number.Object);
	}


	static ConstructorHelpers::FObjectFinder<UMaterialInstance> M_NONE(TEXT("MaterialInstanceConstant'/Game/PROJECT/BP_CLASS/Asset/03_StaticMesh/ShinHo/MI_NoneLED.MI_NoneLED'"));
	if (M_NONE.Succeeded())
		LED_NoneMaterial = M_NONE.Object;
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> M_RED(TEXT("MaterialInstanceConstant'/Game/PROJECT/BP_CLASS/Asset/03_StaticMesh/ShinHo/MI_RedLED.MI_RedLED'"));
	if (M_RED.Succeeded())
		LED_RedMaterial = M_RED.Object;
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> M_GREEN(TEXT("MaterialInstanceConstant'/Game/PROJECT/BP_CLASS/Asset/03_StaticMesh/ShinHo/MI_GreenLED.MI_GreenLED'"));
	if (M_GREEN.Succeeded())
		LED_GreenMaterial = M_GREEN.Object;
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> M_NUMBER(TEXT("MaterialInstanceConstant'/Game/PROJECT/BP_CLASS/Asset/03_StaticMesh/ShinHo/MI_NumberLED.MI_NumberLED'"));
	if (M_NUMBER.Succeeded())
		LED_NumberMaterial = M_NUMBER.Object;

}

// Called when the game starts or when spawned
void ATrafficActor::BeginPlay()
{
	Super::BeginPlay();

	/* 다이나믹 머터리얼 생성 */
	if (LED_NoneMaterial && LED_RedMaterial && LED_GreenMaterial && LED_NumberMaterial)
	{
		_DEBUG("Material");
		MI_None = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), LED_NoneMaterial);
		MI_Red = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), LED_RedMaterial);
		MI_Green = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), LED_GreenMaterial);
		MI_Number_Left = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), LED_NumberMaterial);
		MI_Number_Right = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), LED_NumberMaterial);

		TrafficStart(); // 시작.
	}


}

void ATrafficActor::TrafficStart()
{
	Init(); // 불 끄기
	RedLED(true); // 빨간불 켜기
	//나중에 중앙 신호등을 설치하면 여기서 타이머를 중앙 신호등에서 실행하게하면 된다.
	GetWorld()->GetTimerManager().SetTimer(RedTimer, this, &ATrafficActor::StartGreen, RedTime, false);
}

void ATrafficActor::Init()
{
	RedLED(false);
	GreenLED(false);
	SetNumber(false, 0);
}


void ATrafficActor::RedLED(bool bFlag)
{
	RedLight->SetVisibility(bFlag);
	LEDMesh_Red->SetMaterial(0, bFlag ? MI_Red : MI_None);
}

void ATrafficActor::GreenLED(bool bFlag)
{
	GreenLight->SetVisibility(bFlag);
	LEDMesh_Green->SetMaterial(0, bFlag ? MI_Green : MI_None);
	bGreenOn = bFlag;
}


void ATrafficActor::SetNumber(bool bFlag, int32 Count)
{

	NumberLight->SetVisibility(bFlag);
	LEDMesh_Number_Left->SetMaterial(0, bFlag ? MI_Number_Left : MI_None);
	LEDMesh_Number_Right->SetMaterial(0, bFlag ? MI_Number_Right : MI_None);

	if (!bFlag)
		return;

	if (Count >= 10)
	{
		LEDMesh_Number_Left->SetScalarParameterValueOnMaterials(TEXT("Number"), Count / 10);
		LEDMesh_Number_Right->SetScalarParameterValueOnMaterials(TEXT("Number"), Count % 10);
	}
	else
	{
		LEDMesh_Number_Left->SetMaterial(0, MI_None);
		LEDMesh_Number_Right->SetScalarParameterValueOnMaterials(TEXT("Number"), Count % 10);
	}
}

void ATrafficActor::StartGreen()
{
	RedLED(false);
	GreenLED(true);
	GetWorld()->GetTimerManager().SetTimer(GreenTimer, this, &ATrafficActor::StartCount, GreenTime, false);
}


void ATrafficActor::StartCount()
{
	Cnt = NumberTime;
	GetWorld()->GetTimerManager().SetTimer(NumberCount, [&]()
		{
			if (Cnt > 0)
			{
				--Cnt;
				SetNumber(true, Cnt);
			}
			else
			{
				GetWorld()->GetTimerManager().ClearTimer(NumberCount);
				GetWorld()->GetTimerManager().ClearTimer(GreenOnOff);
				TrafficStart();
			}
		}, 1.0f, true);


	GetWorld()->GetTimerManager().SetTimer(GreenOnOff, this, &ATrafficActor::GreenFlash, 0.5f, true);
	// 초록불 깜빡깜빡

}

void ATrafficActor::GreenFlash()
{
	GreenLED(!bGreenOn);
}