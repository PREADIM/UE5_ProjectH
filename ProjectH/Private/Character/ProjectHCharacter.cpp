// Copyright Epic Games, Inc. All Rights Reserved.

#include "Character/ProjectHCharacter.h"
#include "ProjectHAnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/StaticMeshActor.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ActorComponent/InvisibleWallComponent.h"
#include "ActorComponent/UtilityComponent.h"
#include "ActorComponent/QuestComponent/QuestComponent.h"
#include "Special/TimelineNode.h"
#include "AI/QuestNPCBase.h"
#include "GameMode/ProjectHGameInstance.h"
#include "Controller/ProjectH_PC.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"


DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);


AProjectHCharacter::AProjectHCharacter()
{
	MaxArmLength = 350.f;
	MinArmLength = 70.f;
	MaxArmLocation = FVector(0.f, 0.f, 55.f);
	MinArmLocation = FVector(0.f, 0.f, 70.f);
	SpringArmLength = MaxArmLength;
	SpringArmLocation = MaxArmLocation;

	// ��Ʈ�ѷ��� ȸ������ ����ȭ
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate =  FRotator(0.0f, 720.f, 0.f);

	//1��Ī ī�޶�
	/*Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetCapsuleComponent());->bUsePawnControlRotation = true;
	Camera->SetActive(false);
	Camera->bAutoActivate = false;


	//1��Ī �޽�
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh1P"));
	Mesh1P->SetOnlyOwnerSee(true); // ���� ���̰��ϱ�.
	Mesh1P->SetOwnerNoSee(true);
	Mesh1P->SetupAttachment(Camera);
	Mesh1P->bCastDynamicShadow = false; // �׸��� ����
	Mesh1P->CastShadow = false;*/


	//3��Ī�� ǥ�� �� ��������
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bDoCollisionTest = true;
	SpringArm->SetActive(true);
	SpringArm->TargetArmLength = SpringArmLength;
	

	//3��Ī ī�޶�
	Camera3P = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera3P"));
	Camera3P->SetupAttachment(SpringArm);
	Camera3P->bUsePawnControlRotation = false;
	Camera3P->bAutoActivate = true;
	Camera3P->SetActive(true);


	QuestCollision = CreateDefaultSubobject<USphereComponent>(TEXT("QuestCollision"));
	QuestCollision->SetupAttachment(GetMesh());


	//3��Ī �޽�
	GetMesh()->SetOwnerNoSee(false);
	GetMesh()->bCastHiddenShadow = false;

	//InvisibleComponent = CreateDefaultSubobject<class UInvisibleWallComponent>(TEXT("Invisible"));
	UtilityComponent = CreateDefaultSubobject<class UUtilityComponent>(TEXT("Util"));
	QuestComponent = CreateDefaultSubobject<class UQuestComponent>(TEXT("Quest"));

	//InvisibleComponent->SetActive(false);
	//InvisibleComponent->Activate(false);

	RunningSpeed = 500.f;
	WalkSpeed = 300.f;
}



void AProjectHCharacter::BeginPlay()
{
	Super::BeginPlay();

	UtilityComponent->SetSK_Mesh(GetMesh()); // �ǻ��� ����Ǹ� �ٽ� PoseableMesh�� �����ؾ��ϴ� �Լ��� ������.
	//InvisibleComponent->SetOwnerCamera3P(Camera3P);

	Object.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel4));
	IgnoreActor.Add(this);

	// ���� �ν��Ͻ����� ���� ��������
	if (QuestComponent)
		QuestComponent->StartQuestLoad();

	OwnerController = Cast<class AProjectH_PC>(GetController());
	if (OwnerController)
	{
		MouseSensitivity = OwnerController->MouseSensitivity;
	}
	QuestCollision->OnComponentBeginOverlap.AddDynamic(this, &AProjectHCharacter::QuestCollisionOverlap);
	QuestCollision->OnComponentEndOverlap.AddDynamic(this, &AProjectHCharacter::QuestCollisionEndOverlap);

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
	//AddMovementInput(GetActorForwardVector() * Value);
	/*if (!bStopAnim)
	{
		FVector Forward = FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::X);
		AddMovementInput(Forward, Value);
		if (AnimInstance)
		{
			AnimInstance->ForwardV = Forward * Value;
		}
		
	}*/

	if (bPlay)
	{
		AddMovementInput(FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::X), Value);
	}
}


void AProjectHCharacter::MoveRight(float Value)
{
	//AddMovementInput(GetActorRightVector() * Value);
	/*if (!bStopAnim)
	{
		FVector Right = FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::Y);
		AddMovementInput(Right, Value);
		if (AnimInstance)
		{
			AnimInstance->RightV = Right * Value;
		}
	}*/
	if (bPlay)
	{
		AddMovementInput(FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::Y), Value);
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
		if (NPCActors.Num())
		{
			class AQuestNPCBase* NPC = Cast<class AQuestNPCBase>(NPCActors[0]);
			if (::IsValid(NPC))
			{
				if(NPC->bCanAccept)
					NPC->Interact_Implementation(this);
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

void AProjectHCharacter::Run()
{
	if (AnimInstance)
	{
		AnimInstance->bIsRunning = true;
		GetCharacterMovement()->MaxWalkSpeed = RunningSpeed;
		GetCharacterMovement()->MaxAcceleration = 500.f;
	}
}

void AProjectHCharacter::NotRun()
{
	if (AnimInstance)
	{
		AnimInstance->bIsRunning = false;
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		GetCharacterMovement()->MaxAcceleration = 250.f;
	}
}


void AProjectHCharacter::WheelDown()
{
	if (SpringArmLength == MaxArmLength)
		return;
	SpringArmLength = FMath::Clamp(SpringArmLength + 20.f, MinArmLength, MaxArmLength);
	SpringArm->TargetArmLength = SpringArmLength;
	if (SpringArmLength > 100.f)
	{
		SpringArmLocation = FMath::VInterpTo(SpringArmLocation, MaxArmLocation, GetWorld()->GetDeltaSeconds(), 15.0f);
		SpringArm->SetRelativeLocation(SpringArmLocation);
	}
}

void AProjectHCharacter::WheelUp()
{
	if (SpringArmLength == MinArmLength)
		return;
	SpringArmLength = FMath::Clamp(SpringArmLength - 20.f, MinArmLength, MaxArmLength);
	SpringArm->TargetArmLength = SpringArmLength;
	if (SpringArmLength <= 130.f)
	{
		SpringArmLocation = FMath::VInterpTo(SpringArmLocation, MinArmLocation, GetWorld()->GetDeltaSeconds(), 25.0f);
		SpringArm->SetRelativeLocation(SpringArmLocation);
	}
}

void AProjectHCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Forward", this, &AProjectHCharacter::Forward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AProjectHCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &AProjectHCharacter::LookUp);
	PlayerInputComponent->BindAxis("LookRight", this, &AProjectHCharacter::LookRight);

	PlayerInputComponent->BindAction("InteractKey", IE_Pressed, this, &AProjectHCharacter::InteractKey);
	PlayerInputComponent->BindAction("AnyKey", IE_Pressed, this, &AProjectHCharacter::AnyKey);
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AProjectHCharacter::Run);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &AProjectHCharacter::NotRun);
	PlayerInputComponent->BindAction("WheelUp", IE_Pressed, this, &AProjectHCharacter::WheelUp);
	PlayerInputComponent->BindAction("WheelDown", IE_Pressed, this, &AProjectHCharacter::WheelDown);
}

/*--------------------

	Public Function

---------------------*/


/*void AProjectHCharacter::SetViewChange()
{
	switch (CurrentView)
	{
	case ControllerView::FIRST:
		SetControllerMode(ControllerView::THRID);
		break;
	case ControllerView::THRID:
		SetControllerMode(ControllerView::FIRST);
		break;
	default:
		break;
	}
}*/


/*void AProjectHCharacter::SetControllerMode(ControllerView CView)
{
	switch (CView)
	{
	case ControllerView::FIRST: // 1��Ī���� ����
		CurrentView = ControllerView::FIRST;
		SpringArmLength = MinSpringArmLength;
		bChangeView = true;
		break;

	case ControllerView::THRID: // 3��Ī���� ����
		Camera->SetActive(false);
		Camera3P->SetActive(true);
		Mesh1P->SetOwnerNoSee(true);
		GetMesh()->SetOwnerNoSee(false);
		CurrentView = ControllerView::THRID;
		SpringArmLength = MaxSpringArmLength;
		bChangeView = true;
		break;
	default:
		break;
	}
}*/


void AProjectHCharacter::QuestCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AQuestNPCBase* NPC = Cast<AQuestNPCBase>(OtherActor);
	if (NPC)
	{
		if (NPC->CanVisibleWidget()) // ���� �����Ҽ� �ִ� ����Ʈ���� ���⼭ �˻��ؼ� ����ǥ�� ���� �ɵ��ϴ�.
		{
			NPC->SetActorTickEnabled(true);
			NPC->PlayerCharacter = this;
			NPC->SetIconWidget();
		}
		else
		{
			if (NPC->FindCanQuest())
			{
				NPC->SetActorTickEnabled(true);
				NPC->PlayerCharacter = this;
				NPC->SetIconWidget();
			}
		}
	}
	
}

void AProjectHCharacter::QuestCollisionEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AQuestNPCBase* NPC = Cast<AQuestNPCBase>(OtherActor);
	if (NPC)
	{
		NPC->SetActorTickEnabled(false);
		NPC->HiddenIcon();
	}
}

/* ����Ʈ �ݸ����� ũ�⸦ �ٿ��ٰ� �ٽ� �÷��� ���� ������ �ǰ��ϴ� Ʈ��
	����Ʈ�� �ǽð����� �߰��Ҷ� �� �Լ��� �����ؾ� ����ǥ�� ��� �� �ִ�. �ڡ�*/
void AProjectHCharacter::QuestCollisionSetUp()
{
	QuestCollision->SetSphereRadius(1.0f);
	QuestCollision->SetSphereRadius(6000.0f);
}


/*---------------------
	virtual Function
---------------------*/

void AProjectHCharacter::Tick(float DeltaTime)
{
	/*if (bChangeView)
	{
		SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, SpringArmLength, DeltaTime, 5.0f);

		if (SpringArm->TargetArmLength <= MinSpringArmLength + 10.f) // 1��Ī�� �������� �� �پ��� ī�޶� ���ؾ��ϹǷ� ƽ�� ����.
		{
			Camera3P->SetActive(false);
			Camera->SetActive(true);
			Mesh1P->SetOwnerNoSee(false);
			GetMesh()->SetOwnerNoSee(true);
			bChangeView = false;
		}
		else
		{
			if (SpringArm->TargetArmLength == SpringArmLength)
				bChangeView = false;
		}
	}*/

	
	bCanInteract = UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(), GetActorLocation(), 150.f, Object, nullptr, IgnoreActor, NPCActors);

}



