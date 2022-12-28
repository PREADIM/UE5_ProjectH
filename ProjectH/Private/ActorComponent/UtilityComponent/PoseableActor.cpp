// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/UtilityComponent/PoseableActor.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Components/PoseableMeshComponent.h"

// Sets default values
APoseableActor::APoseableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PoseableMesh = CreateDefaultSubobject<UPoseableMeshComponent>(TEXT("PoseableMesh"));
	RootComponent = PoseableMesh;

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> M_Poseable(TEXT("MaterialInstanceConstant'/Game/PROJECT/Material/Poseable/MI_Poseable.MI_Poseable'"));
	if (M_Poseable.Succeeded())
	{
		PoseableMaterial = M_Poseable.Object;
	}

}

// Called when the game starts or when spawned
void APoseableActor::BeginPlay()
{
	Super::BeginPlay();

	if (PoseableMaterial)
	{
		TArray<UMaterialInterface*> Mats = PoseableMesh->GetMaterials();
		Materials = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), PoseableMaterial);

		for (int i = 0; i < Mats.Num(); i++)
			PoseableMesh->SetMaterial(i, Materials);

		bSpawned = true;
		FadeCnt = FadeOutTime; // �������Ʈ���� ������ Fade�ƿ��� �����ؼ� Tick���� �����δ�.
	}

}

// Called every frame
void APoseableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bSpawned)
	{
		FadeCnt -= DeltaTime;

		Materials->SetScalarParameterValue("Opacity", FadeCnt / FadeOutTime); // ��� ���͸����� Materials �ν��Ͻ��� �־����Ƿ� �ϳ��� ��ȭ�ص� �� ����.

		if (FadeCnt < 0)
			Destroy(); // �׳� �� ���͸� �ٷ� ���� ��Ų��.
	}



}

void APoseableActor::SetPoseMesh(USkeletalMeshComponent* Mesh)
{
	PoseableMesh->SetSkeletalMesh(Mesh->GetSkeletalMeshAsset());
}


void APoseableActor::SetCopyPose(USkeletalMeshComponent* Mesh)
{
	PoseableMesh->CopyPoseFromSkeletalComponent(Mesh);
}
