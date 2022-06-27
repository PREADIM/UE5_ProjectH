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
#include "UI/InteractWidget.h"
#include "UI/MainQuestUI.h"
#include "Components/Button.h"



DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);


AProjectHCharacter::AProjectHCharacter()
{
	MaxArmLength = 350.f;
	MinArmLength = 70.f;
	MaxArmLocation = FVector(0.f, 0.f, 55.f);
	MinArmLocation = FVector(0.f, 0.f, 70.f);
	SpringArmLength = MaxArmLength;
	SpringArmLocation = MaxArmLocation;

	// 컨트롤러의 회전값에 동기화
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate =  FRotator(0.0f, 720.f, 0.f);

	//1인칭 카메라
	/*Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetCapsuleComponent());->bUsePawnControlRotation = true;
	Camera->SetActive(false);
	Camera->bAutoActivate = false;


	//1인칭 메시
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh1P"));
	Mesh1P->SetOnlyOwnerSee(true); // 나만 보이게하기.
	Mesh1P->SetOwnerNoSee(true);
	Mesh1P->SetupAttachment(Camera);
	Mesh1P->bCastDynamicShadow = false; // 그림자 제거
	Mesh1P->CastShadow = false;*/


	//3인칭을 표현 할 스프링암
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bDoCollisionTest = true;
	SpringArm->SetActive(true);
	SpringArm->TargetArmLength = SpringArmLength;
	

	//3인칭 카메라
	Camera3P = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera3P"));
	Camera3P->SetupAttachment(SpringArm);
	Camera3P->bUsePawnControlRotation = false;
	Camera3P->bAutoActivate = true;
	Camera3P->SetActive(true);


	QuestCollision = CreateDefaultSubobject<USphereComponent>(TEXT("QuestCollision"));
	QuestCollision->SetupAttachment(GetMesh());
	InteractCollision = CreateDefaultSubobject<USphereComponent>(TEXT("InteractCollision"));
	InteractCollision->SetupAttachment(GetMesh());
	InteractCollision->SetSphereRadius(150.f);


	//3인칭 메시
	GetMesh()->SetOwnerNoSee(false);
	GetMesh()->bCastHiddenShadow = false;

	//InvisibleComponent = CreateDefaultSubobject<class UInvisibleWallComponent>(TEXT("Invisible"));
	UtilityComponent = CreateDefaultSubobject<class UUtilityComponent>(TEXT("Util"));
	QuestComponent = CreateDefaultSubobject<class UQuestComponent>(TEXT("Quest"));

	
	RunningSpeed = 500.f;
	WalkSpeed = 300.f;	
}



void AProjectHCharacter::BeginPlay()
{
	Super::BeginPlay();

	UtilityComponent->SetSK_Mesh(GetMesh()); // 의상이 변경되면 다시 PoseableMesh를 변경해야하니 함수로 뺴놓음.
	//InvisibleComponent->SetOwnerCamera3P(Camera3P);

	/*Object.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel4));
	IgnoreActor.Add(this);*/

	// 게임 인스턴스에서 정보 가져오기
	if (QuestComponent)
		QuestComponent->StartQuestLoad();

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
		AddMovementInput(FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::X), Value);
	}
}


void AProjectHCharacter::MoveRight(float Value)
{
	//AddMovementInput(GetActorRightVector() * Value);

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
		if (InteractNPCActor)
		{		
			if (InteractNPCActor->bCanAccept)
			{
				if (OwnerController)
				{
					InteractNPCActor->Interact_Implementation(this);
					OwnerController->MainQuestUI->CloseInteract();
				}
				
			}	
		}
	}

}

/* 아무키 입력 이벤트 */
void AProjectHCharacter::AnyKey(FKey Key)
{
	if (OwnerController)
	{
		if (OwnerController->CurrentEventInfo.State == EQTEState::Pending)
		{
			if (Key == OwnerController->CurrentEventInfo.Key) // 현재 QTE 이벤트의 키와 같으면
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
	case ControllerView::FIRST: // 1인칭으로 변경
		CurrentView = ControllerView::FIRST;
		SpringArmLength = MinSpringArmLength;
		bChangeView = true;
		break;

	case ControllerView::THRID: // 3인칭으로 변경
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
		if (NPC->CanVisibleWidget()) // 내가 수행할수 있는 퀘스트인지 여기서 검사해서 느낌표를 띄우면 될듯하다.
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

void AProjectHCharacter::InteractCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AQuestNPCBase* NPC = Cast<AQuestNPCBase>(OtherActor);
	if (NPC)
	{
		if (InteractNPCActor == NPC)
		{
			return;
		}

		if (bCanInteract)
		{
			InteractNPCActor = NPC;
			OwnerController->MainQuestUI->SetName(NPC->NPCName);
		}
		else
		{
			bCanInteract = true;
			InteractNPCActor = NPC;	
			OwnerController->MainQuestUI->SetName(NPC->NPCName);
		}	

		OwnerController->MainQuestUI->OpenInteract();
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
}


/* 퀘스트 콜리전의 크기를 줄였다가 다시 늘려서 새로 오버랩 되게하는 트릭
	퀘스트를 실시간으로 추가할때 이 함수를 실행해야 느낌표를 띄울 수 있다. ★★*/
void AProjectHCharacter::QuestCollisionSetUp()
{
	QuestCollision->SetSphereRadius(1.0f);
	QuestCollision->SetSphereRadius(6000.0f);
}


void AProjectHCharacter::InteractCollisionSetUp()
{
	InteractCollision->SetSphereRadius(0.5f);
	InteractCollision->SetSphereRadius(150.0f);
}

/*---------------------
	virtual Function
---------------------*/

void AProjectHCharacter::Tick(float DeltaTime)
{
	/*if (bChangeView)
	{
		SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, SpringArmLength, DeltaTime, 5.0f);

		if (SpringArm->TargetArmLength <= MinSpringArmLength + 10.f) // 1인칭은 스프링이 다 줄어들고 카메라가 변해야하므로 틱에 넣음.
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

	
	/*bCanInteract = UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(), GetActorLocation(), 150.f, Object, nullptr, IgnoreActor, NPCActors);*/
	

}



