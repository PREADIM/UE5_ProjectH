// Copyright Epic Games, Inc. All Rights Reserved.

#include "Character/ProjectHCharacter.h"
#include "ProjectHAnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ActorComponent/QuestComponent/QuestComponent.h"
#include "AI/QuestNPCBase.h"
#include "GameMode/ProjectHGameInstance.h"
#include "Controller/ProjectH_PC.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/InteractWidget.h"
#include "UI/MainQuestUI.h"
#include "Components/Button.h"
#include "ActorComponent/QuestComponent/TriggerSpawnActor.h"



DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);


AProjectHCharacter::AProjectHCharacter()
{
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetMesh());
	Camera->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("head"));

	QuestCollision = CreateDefaultSubobject<USphereComponent>(TEXT("QuestCollision"));
	QuestCollision->SetupAttachment(RootComponent);
	InteractCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractCollision"));
	InteractCollision->SetupAttachment(Camera);
	InteractCollision->SetBoxExtent(InteractBoxSize);

	// ��Ʈ�ѷ��� ȸ������ ����ȭ
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate =  FRotator(0.0f, 720.f, 0.f);
	
	//3��Ī �޽�
	GetMesh()->SetOwnerNoSee(false);
	GetMesh()->bCastHiddenShadow = false;

	QuestComponent = CreateDefaultSubobject<class UQuestComponent>(TEXT("Quest"));
	
	//RunningSpeed = 500.f;
	WalkSpeed = 150.f;	

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	//������ �Ǿ����� �� �ڿ� �ִ��� �Ǵ��ϱ� ���� ��.
	
}



void AProjectHCharacter::BeginPlay()
{
	Super::BeginPlay();
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel4));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel16));
	IgnoreActor.Add(this);

	// ���� �ν��Ͻ����� ���� ��������
	if (QuestComponent)
	{
		QuestComponent->StartQuestLoad(); // �ε�
		QuestComponent->BeginSetupHaveQuests();
	}

	OwnerController = Cast<class AProjectH_PC>(GetController());
	if (OwnerController)
	{
		MouseSensitivity = OwnerController->MouseSensitivity;
		OwnerController->MainQuestUI->InteractWidget->InteractButton->OnClicked.AddDynamic(this, &AProjectHCharacter::InteractKey);
	}

	QuestCollision->OnComponentBeginOverlap.AddDynamic(this, &AProjectHCharacter::QuestCollisionOverlap);
	QuestCollision->OnComponentEndOverlap.AddDynamic(this, &AProjectHCharacter::QuestCollisionEndOverlap);
	InteractCollision->OnComponentBeginOverlap.AddDynamic(this, &AProjectHCharacter::InteractCollisionOverlap);
	InteractCollision->OnComponentEndOverlap.AddDynamic(this, &AProjectHCharacter::InteractCollisionEndOverlap);

	QuestCollisionSetUp();
	InteractCollisionSetUp();
}

void AProjectHCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AnimInstance = Cast<class UProjectHAnimInstance>(GetMesh()->GetAnimInstance());
}


void AProjectHCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	QuestComponent->UpdateQuestSave();
}


/*----------------
	Move & Utils
-----------------*/
void AProjectHCharacter::Forward(float Value)
{
	if (bPlay)
	{
		//AddMovementInput(FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::X), Value);
		FVector Direction = FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::X);
		Direction.Z = 0.f;
		Direction.Normalize();
		AddMovementInput(Direction, Value);
	}
}


void AProjectHCharacter::MoveRight(float Value)
{
	//AddMovementInput(GetActorRightVector() * Value);

	if (bPlay)
	{
		//AddMovementInput(FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::Y), Value);
		FVector Direction = FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::Y);
		Direction.Z = 0.f;
		Direction.Normalize();
		AddMovementInput(Direction, Value);
	}
	
}


void AProjectHCharacter::LookUp(float AxisValue)
{
	if(!bQTE)
		AddControllerPitchInput(AxisValue * MouseSensitivity * GetWorld()->GetDeltaSeconds());
}


void AProjectHCharacter::LookRight(float AxisValue)
{
	if (!bQTE)
		AddControllerYawInput(AxisValue * MouseSensitivity * GetWorld()->GetDeltaSeconds());
}

void AProjectHCharacter::InteractKey()
{
	if (bCanInteract)
	{
		if (InteractNPCActor)
		{	
			if (OwnerController)
			{
				InteractNPCActor->Interact_Implementation(this);
				OwnerController->MainQuestUI->CloseInteract();
			}
		}
		else if(InteractActor) // Ʈ���ſ� ���� ������ ���͸� ���ͷ�Ʈ.
		{	
			if (OwnerController)
			{

				InteractActor->Interact_Implementation(this);
				//OwnerController->MainQuestUI->CloseInteract();
				InteractCollisionSetUp();
				
			}
		}
	}

}

/* �ƹ�Ű �Է� �̺�Ʈ */
void AProjectHCharacter::AnyKey(FKey Key)
{
	if (OwnerController)
	{
		if (OwnerController->CurrentEventInfo.State == EQTEState::Pending)
		{
			if (Key == OwnerController->CurrentEventInfo.Key) // ���� QTE �̺�Ʈ�� Ű�� ������
				OwnerController->CurrentEventInfo.State = EQTEState::Success;
			else
				OwnerController->CurrentEventInfo.State = EQTEState::FailKey;
		}
	}
	
}

//void AProjectHCharacter::Run()
//{
//	if (AnimInstance)
//	{
//		AnimInstance->bIsRunning = true;
//		GetCharacterMovement()->MaxWalkSpeed = RunningSpeed;
//		GetCharacterMovement()->MaxAcceleration = 500.f;
//	}
//}
//
//void AProjectHCharacter::NotRun()
//{
//	if (AnimInstance)
//	{
//		AnimInstance->bIsRunning = false;
//		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
//		GetCharacterMovement()->MaxAcceleration = 250.f;
//	}
//}


void AProjectHCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Forward", this, &AProjectHCharacter::Forward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AProjectHCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &AProjectHCharacter::LookUp);
	PlayerInputComponent->BindAxis("LookRight", this, &AProjectHCharacter::LookRight);

	PlayerInputComponent->BindAction("InteractKey", IE_Pressed, this, &AProjectHCharacter::InteractKey);
	PlayerInputComponent->BindAction("AnyKey", IE_Pressed, this, &AProjectHCharacter::AnyKey);
	/*PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AProjectHCharacter::Run);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &AProjectHCharacter::NotRun);*/
}

/*--------------------

	Public Function

---------------------*/



void AProjectHCharacter::QuestCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AQuestNPCBase* NPC = Cast<AQuestNPCBase>(OtherActor);
	if (NPC)
	{
		if (NPC->NPCQuestSetup())
		{
			NPC->PlayerCharacter = this;
		}
	}
}

void AProjectHCharacter::QuestCollisionEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AQuestNPCBase* NPC = Cast<AQuestNPCBase>(OtherActor);
	if (NPC)
	{
		NPC->HiddenIcon();
	}

	//QuestCollisionSetUp(); ���ͷ�Ʈ�ʹ� �ٸ��� ����Ʈ �ݸ����� ���� �ٽ� �¾� �����൵�ɵ��ϴ�.
}

void AProjectHCharacter::InteractCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FVector StartLocation = Camera->GetComponentLocation();
	FVector EndLocation = OtherComp->GetComponentLocation();
	FHitResult HitResult;
	
	bool bTrace = UKismetSystemLibrary::BoxTraceSingleForObjects(GetWorld(), StartLocation, EndLocation, InteractBoxSize, Camera->GetComponentRotation(), ObjectTypes, false, IgnoreActor, EDrawDebugTrace::None, HitResult, true);
	if (bTrace)
	{
		if (OtherComp != HitResult.GetComponent())
			return;

		AQuestNPCBase* NPC = Cast<AQuestNPCBase>(OtherActor);
		if (NPC)
		{
			if (InteractNPCActor == NPC)
				return;


			bCanInteract = true;
			InteractNPCActor = NPC;
			OwnerController->MainQuestUI->SetName(NPC->NPCName);
			OwnerController->MainQuestUI->OpenInteract();
			return;
		}
		else
		{
			ATriggerSpawnActor* TriggerActor = Cast<ATriggerSpawnActor>(OtherActor);
			if (TriggerActor)
			{
				if (InteractActor == TriggerActor)
					return;

				if (TriggerActor->bCanThisInteract)
				{
					bCanInteract = true;
					InteractActor = TriggerActor;
					InteractActor->PlayerCharacter = this;
					OwnerController->MainQuestUI->SetName(InteractActor->TriggerText);
					OwnerController->MainQuestUI->OpenInteract();
					return;
				}
			}
		}
	}
}

void AProjectHCharacter::InteractCollisionEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AQuestNPCBase* NPC = Cast<AQuestNPCBase>(OtherActor);
	if (NPC)
	{
		bCanInteract = false;
		if (InteractNPCActor == NPC)
		{
			InteractNPCActor = nullptr;
			OwnerController->MainQuestUI->CloseInteract();
		}
	}
	else
	{
		ATriggerSpawnActor* TriggerActor = Cast<ATriggerSpawnActor>(OtherActor);
		if (TriggerActor)
		{
			bCanInteract = false;
			if (InteractActor == TriggerActor)
			{
				InteractActor = nullptr;
				OwnerController->MainQuestUI->CloseInteract();
			}
		}
	}

	InteractCollisionSetUp();
}


void AProjectHCharacter::QuestCollisionSetUp()
{
	_DEBUG("Quest Collision Setup");
	QuestCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//Ÿ�̸�
	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, FTimerDelegate::CreateLambda([&]()
		{
			QuestCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		}), 0.2f, false);
}


void AProjectHCharacter::InteractCollisionSetUp()
{
	_DEBUG("Interact Collision Setup");
	InteractCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, FTimerDelegate::CreateLambda([&]()
		{
			InteractCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		}), 0.2f, false);	
}

/*---------------------
	virtual Function
---------------------*/

void AProjectHCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



