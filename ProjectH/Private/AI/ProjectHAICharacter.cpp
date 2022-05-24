// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ProjectHAICharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "AI/ProjectHAIController.h"


// Sets default values
AProjectHAICharacter::AProjectHAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	AIControllerClass = AProjectHAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BP_BT(TEXT("BehaviorTree'/Game/PROJECT/BP_CLASS/Blueprints/03_AI/Dummy_BT.Dummy_BT'"));
	if (BP_BT.Succeeded())
		BT = BP_BT.Object;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);

}


void AProjectHAICharacter::BeginPlay()
{
	Super::BeginPlay();


}


// Called every frame
void AProjectHAICharacter::Tick(float DeltaTime)
{


}




void AProjectHAICharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

}