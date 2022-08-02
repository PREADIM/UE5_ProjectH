// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/MainUI/PartySettingField.h"
#include "Tema/JRPG/JRPGPlayerController.h"
#include "Tema/JRPG/JRPGUnit.h"
#include "Tema/JRPG/MainUI/JRPGSettingPartySlot.h"
#include "Tema/JRPG/MainUI/JRPGPartySettingUI.h"
#include "Tema/JRPG/JRPGGameMode.h"
#include "Tema/JRPG/CustomWidget.h"


// Sets default values
APartySettingField::APartySettingField()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	Unit1 = CreateDefaultSubobject<USceneComponent>(TEXT("Unit1"));
	Unit2 = CreateDefaultSubobject<USceneComponent>(TEXT("Unit2"));
	Unit3 = CreateDefaultSubobject<USceneComponent>(TEXT("Unit3"));
	Unit4 = CreateDefaultSubobject<USceneComponent>(TEXT("Unit4"));


	RootComponent = Root;
	Camera->SetupAttachment(Root);

	Unit1->SetupAttachment(Root);
	Unit2->SetupAttachment(Root);
	Unit3->SetupAttachment(Root);
	Unit4->SetupAttachment(Root);


	TEnumAsByte<EObjectTypeQuery> OT = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel8);
	ObjectTypes.Add(OT);

	CurrentLocation = FVector(239.f, 67.f,143.f);
	TargetLocation = FVector(239.f, 67.f, 143.f);

	SpawnChars.Init(nullptr, 4);
}

// Called when the game starts or when spawned
void APartySettingField::BeginPlay()
{
	Super::BeginPlay();



}

// Called every frame
void APartySettingField::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentLocation != TargetLocation)
	{
		CurrentLocation = FMath::VInterpTo(CurrentLocation, TargetLocation, DeltaTime, 5.0f);
		Camera->SetRelativeLocation(CurrentLocation);
	}

}

// Called to bind functionality to input
void APartySettingField::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	InputComponent->BindAction(TEXT("LMB"), IE_Released, this, &APartySettingField::LMB);
}


void APartySettingField::Init(class UJRPGMainWidget* OwnerMainUI)
{
	if (OwnerController && BP_SettingUI && BP_PartySettingUI)
	{
		GM = OwnerController->GM;
		PartySettingUI = CreateWidget<UJRPGPartySettingUI>(GetWorld(), BP_PartySettingUI);
		SettingUI = CreateWidget<UJRPGSettingPartySlot>(GetWorld(), BP_SettingUI);
		if (PartySettingUI && SettingUI)
		{
			PartySettingUI->OwnerController = OwnerController;
			PartySettingUI->OwnerMainUI = OwnerMainUI;
			PartySettingUI->Init();
			PartySettingUI->PartyField = this;
			
			SettingUI->OwnerController = OwnerController;
			SettingUI->OwnerField = this;
			SettingUI->Init();
	
			UE_LOG(LogTemp, Warning, TEXT("Init"));
		}
	}
}

void APartySettingField::SetCurrentParty()
{
	if (PartySettingUI && OwnerController)
	{

		SpawnCharacter();

		OwnerController->LastWidget.AddUnique(PartySettingUI);
		PartySettingUI->AddToViewport();
		UE_LOG(LogTemp, Warning, TEXT("SetCurrentParty"));
	}

}


void APartySettingField::SpawnCharacter()
{
	CurrentParty = OwnerController->CurrentParty;

	for (int32 i = 0; i < 4; i++)
	{
		FTransform UnitLocation;
		switch (i)
		{
		case 0:
			UnitLocation = Unit1->GetComponentTransform();
			break;
		case 1:
			UnitLocation = Unit2->GetComponentTransform();
			break;
		case 2:
			UnitLocation = Unit3->GetComponentTransform();
			break;
		case 3:
			UnitLocation = Unit4->GetComponentTransform();
			break;
		}

		if (CurrentParty.IsValidIndex(i))
		{
			SpawnChars[i] = GM->GetCharacterSpawn(CurrentParty[i], UnitLocation);
		}
		else
		{
			SpawnChars[i] = GM->GetCharacterSpawn(0, UnitLocation);
		}

	}
}


/* ��Ƽ���ÿ��� ��Ƽ�� ���������� �ش� ĭ�� �ǽð����� ���� �����ϴ� �Լ�.*/
void APartySettingField::SetSpawnUnit(int32 Number)
{
	FTransform UnitLocation;
	switch (Number)
	{
	case 0:
		UnitLocation = Unit1->GetComponentTransform();
		break;
	case 1:
		UnitLocation = Unit2->GetComponentTransform();
		break;
	case 2:
		UnitLocation = Unit3->GetComponentTransform();
		break;
	case 3:
		UnitLocation = Unit4->GetComponentTransform();
		break;
	}

	if (SpawnChars[Number] != nullptr)
	{
		SpawnChars[Number]->Destroy();
		SpawnChars[Number] = GM->GetCharacterSpawn(CurrentParty[Number], UnitLocation);
	}
	else
	{
		SpawnChars[Number] = GM->GetCharacterSpawn(CurrentParty[Number], UnitLocation);
	}
	

}


// ��Ƽ ĳ������ ����� ����Ǿ����Ƿ�, �ٽ� ����
void APartySettingField::SetPartyList(int32 CharNum)
{
	// ������ ĳ���͸� �����Ҷ�
	if (SpawnChars[SelectNumber]->CharNum == CharNum)
	{
		// JRPGSettingPartySlot ������ �����ϱ� ��ư�� Visible �ϱ�.

	}
	else if (SpawnChars[SelectNumber]->CharNum != CharNum)
	{
		SetSpawnUnit(SelectNumber); // �����ֱ�.
		// JRPGSettingPartySlot ������ �����ϱ� ��ư�� Visible �ϱ�.
		// �����ϱ� ��ư�� ������ ����Ʈ ��ü.
		
	}

	
}


// ��Ƽ ĳ������ ����� ����Ǿ����Ƿ�, ĳ���� ��ġ �缳��
// �ڡ� �����ϱ⳪ �����ϱ� �������� ����ǰ��Ѵ�.
void APartySettingField::SetPartyChar()
{
	SpawnCharacter(); // ��Ʈ�ѷ��� CurrentParty�� �ٽ� ���Ӱ� �޾ƿ��鼭 ���� �����ϱ�.
	ResomeUI();
}



void APartySettingField::LMB()
{
	FHitResult HitResult;


	bool bHit = OwnerController->GetHitResultUnderCursorForObjects(ObjectTypes, false, HitResult);
	if (bHit)
	{
		class AJRPGUnit* LastUnit = Cast<class AJRPGUnit>(HitResult.GetActor());
		if (LastUnit)
		{
			_DEBUG("%s", *LastUnit->GetName());

			for (int32 i = 0; i < SpawnChars.Num(); i++)
			{
				if (LastUnit == SpawnChars[i])
				{
					switch (i)
					{
					case 0:
						TargetLocation = Unit1_CameraTransform;
						break;
					case 1:
						TargetLocation = Unit2_CameraTransform;
						break;
					case 2:
						TargetLocation = Unit3_CameraTransform;
						break;
					case 3:
						TargetLocation = Unit4_CameraTransform;
						break;
					}

					SelectNumber = i; // �̰� ����ϰ��־�� ĳ���� �������� ���������� �ش� ĭ�� ���氡��
 					break;
				}
			}

			if (PartySettingUI)
			{
				PartySettingUI->SetVisibility(ESlateVisibility::Hidden);
				OwnerController->LastWidget.AddUnique(SettingUI);

				if (!SettingUI->IsInViewport())
					SettingPartySlot();			
			}	
		}
	}
}


// ��Ƽ �����ϴ� UI ����.
void APartySettingField::SettingPartySlot()
{
	if (SettingUI)
	{
		SettingUI->AddToViewport();
	}
}


//SettingSlotUI���� �ٽ� ��ó�� ��Ƽ UI �������� ���ư���.
void APartySettingField::ResomeUI()
{
	PartySettingUI->SetVisibility(ESlateVisibility::Visible);
	TargetLocation = ZeroLocation;
}