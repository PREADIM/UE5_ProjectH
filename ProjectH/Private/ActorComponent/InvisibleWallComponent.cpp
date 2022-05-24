// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/InvisibleWallComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/StaticMeshActor.h"
#include "Character/ProjectHCharacter.h"
#include "Controller/ProjectH_PC.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMaterialLibrary.h"


// Sets default values for this component's properties
UInvisibleWallComponent::UInvisibleWallComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> BP_M(TEXT("MaterialInstanceConstant'/Game/PROJECT/Material/InvisibleWall/MI_InvisibleWall.MI_InvisibleWall'"));
	if (BP_M.Succeeded())
		InvisibleMaterial = BP_M.Object;


	// ...
}


// Called when the game starts
void UInvisibleWallComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<AProjectHCharacter>(GetOwner());
	if (OwnerCharacter)
		OwnerController = Cast<AProjectH_PC>(OwnerCharacter->GetController());

	CameraObjectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel3);
	CameraQueryParams.AddIgnoredActor(GetOwner());

	if (InvisibleMaterial)
		IMaterial = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), InvisibleMaterial);

	// ...
	
}


// Called every frame
void UInvisibleWallComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (OwnerCharacter)
	{
		/*if (OwnerCharacter->bTPS())
		{
			bMesh = CameraTraceByWall();
			if (!bMesh)
				ResetMaterial(); // 전부 원상복귀 하는 함수.
		}
		else
		{
			if(bMesh) // 1인칭인데 아직 머터리얼이 초기화가 다 안됨.
				ResetMaterial(); // 1인칭으로 변환해도 적용되게
		}*/

		// 특정 시점이 되었을때 스프링암의 콜리전테스트를 끄고 CameraTraceByWall를 사용한다.
	}
	// ...
}


/*------------------

	Public Function

--------------------*/

/* 3인칭 카메라와 캐릭터 사이에 시야를 가리는 메시를 찾는다. */
bool UInvisibleWallComponent::CameraTraceByWall()
{
	TArray<FHitResult> HitResult;
	bool bHitTraceByWall;

	if (OwnerCamera3P && OwnerCharacter)
	{
		Camera3PStart = OwnerCamera3P->GetComponentLocation();
		Camera3PEnd = OwnerCharacter->GetActorLocation() + FVector(0.f, 0.f, 90.f);
	}

	bHitTraceByWall = GetWorld()->LineTraceMultiByObjectType(HitResult, Camera3PStart, Camera3PEnd, CameraObjectParams, CameraQueryParams);
	if (bHitTraceByWall)
	{
		auto PrevMeshes = HittedMesh;

		for (auto WallResult : HitResult)
		{
			AStaticMeshActor* OtherMesh = Cast<AStaticMeshActor>(WallResult.GetActor());
			if (OtherMesh && !HittedMesh.Contains(OtherMesh))
			{
				//DrawDebugPoint(GetWorld(), WallResult.Location, 20, FColor::Blue, false);
				//SCREEN_DEBUG(3.0f, FColor::Black, "Name : %s", *OtherMesh->GetStaticMeshComponent()->GetName()); // 로그
				StaticMeshInvisible(OtherMesh, 0.15f); // 머터리얼을 바꿔주는 함수.
				HittedMesh.AddUnique(OtherMesh);	
			}
			PrevMeshes.RemoveSingle(OtherMesh);
		}


		if (PrevMeshes.IsEmpty()) // 비어있으면 지울게 없다.
			return true;
		else
		{
			if (HittedMesh.Num() != PrevMeshes.Num())
			{
				for (auto ResetWall : PrevMeshes)
				{
					Reset(ResetWall); //더이상 가리키지 않는 메쉬를 원상복구하는 함수.
					HittedMesh.RemoveSingle(ResetWall);
				}
				return true;
			}
		}

		return true;
	}
	DrawDebugLine(GetWorld(), Camera3PStart, Camera3PEnd, FColor::Red, false);

	return false;
}


/* CameraTraceByWall로 찾은 메쉬들을 DeltaTime으로 투명화한다. */
void UInvisibleWallComponent::StaticMeshInvisible(AStaticMeshActor* StaticMesh, float SetOpacity)
{
	UStaticMeshComponent* M = StaticMesh->GetStaticMeshComponent();
	OriginalMaterials.Add(StaticMesh);
	OriginalMaterials[StaticMesh] = M->GetMaterials();
	for (int i = 0; i < OriginalMaterials[StaticMesh].Num(); i++)
	{
		M->SetMaterial(i, IMaterial);
	}

	IMaterial->SetScalarParameterValue(TEXT("Opacity"), SetOpacity);

}


void UInvisibleWallComponent::Reset(AStaticMeshActor* ResetWall)
{
	UStaticMeshComponent* M = ResetWall->GetStaticMeshComponent();
	if (M)
	{
		for (int i = 0; i < OriginalMaterials[ResetWall].Num(); i++)
		{
			M->SetMaterial(i, OriginalMaterials[ResetWall][i]);
		}
	}
}


/* 투명화 되어있는 Mesh들을 다시 원래대로 돌려놓는다. */
void UInvisibleWallComponent::ResetMaterial()
{
	if (!HittedMesh.IsEmpty()) // 투명화된 메쉬가 있으면
	{
		for (auto Walls : HittedMesh)
		{
			UStaticMeshComponent* M = Walls->GetStaticMeshComponent();
			for (int i = 0; i < OriginalMaterials[Walls].Num(); i++)
			{
				M->SetMaterial(i, OriginalMaterials[Walls][i]);
			}
			
		}
	

		HittedMesh.Empty();
		bMesh = false; // 투명 머터리얼 실행중인 메쉬없음.
	}


	// SingleLine Trace

	/*if (HittedMesh != nullptr)
	{
		UStaticMeshComponent* M = HittedMesh->GetStaticMeshComponent();
		for (int i = 0; i< OriginalMaterials.Num(); i++)
		{
			M->SetMaterial(i, OriginalMaterials[i]);
		}

		OriginalMaterials.Empty();
		HittedMesh = nullptr;
	}*/
}
