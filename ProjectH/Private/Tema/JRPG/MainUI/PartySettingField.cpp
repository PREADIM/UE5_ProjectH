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
			SettingUI->GM = GM;
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
	for(int32 i = 0; i < 4; i++)
	{
		if (SpawnChars[i] != nullptr)
		{
			SpawnChars[i]->Destroy();
		}
	}


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
void APartySettingField::SetSpawnUnit(int32 Number, int32 CharNum)
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
		SpawnChars[Number] = GM->GetCharacterSpawn(CharNum, UnitLocation); // ������ ĳ���� �̸� ����
		// ��Ʈ�ѷ��� CurrentParty�� �ǵ�������.
	}
	else
	{
		SpawnChars[Number] = GM->GetCharacterSpawn(CharNum, UnitLocation);
	}
	

}


// ��Ƽ ĳ������ ����� ����Ǿ����Ƿ�, �ٽ� ����
void APartySettingField::SetPartyList(int32 CharNum)
{
	if (SettingUI)
	{
		// �ƿ� ���� �߰��ϴ� ����
		if (!OwnerController->CurrentParty.IsValidIndex(SelectNumber))
		{

			bool bCan = true; // �̹� ��Ƽ�� ������ ���ʿ� x
			for (int32 i = 0; i < OwnerController->CurrentParty.Num(); i++)
			{
				if (CharNum == SpawnChars[i]->CharNum)
				{
					bCan = false; // �̹� �����Ƿ� ���� ����
					//���⼭ �̹� �����Ѵٰ� ���â ����.
					// OwnerController->LastWidget�� �ֱ�.
					break;
				}
			}

			if (bCan)
			{
				SetSpawnUnit(SelectNumber, CharNum);
				SettingUI->SetVisibilityOutButton(false); // �����ϱ� ����.
				SettingUI->SetVisibilitySelectButton(true); // �����ϱ� �ѱ�.
				SettingUI->SetCharNum = CharNum;
				SettingUI->SetPartyChange(); // ���Ӱ� �߰��ϰ� �ٷ� ������.
			}
			return;
		}

		// ���� ���ǹ��� �ƿ� ���� �߰��ϹǷ� �Ʒ� ������ ������ �ʿ䰡 x

		if (SpawnChars[SelectNumber]->CharNum == CharNum && CharNum != 0) // ������ �����ϸ� ���ʿ� �����ϱ⸸ ������. ���� �� �ʿ�x
		{
			SettingUI->SetVisibilitySelectButton(false); // �����ϱ� ����
			SettingUI->SetVisibilityOutButton(true); // �����ϱ� �ѱ�.

			// ������ �ʿ�x �Ʒ��� ���� �� �ִ� ������, 
			// �ٸ��� �����ٰ�, �ٽ� ���� ���� ĳ���͸� �������� �����ϱⰡ �߰��ϱ� �����̴�.
		}		
		else if (SpawnChars[SelectNumber]->CharNum != CharNum && CharNum != 0)
		{
			// CurrentParty�� �ٸ���, ���� ĳ���Ϳ͵� �ٸ��ٴ� ����, ������ ���õ� ĳ���͵� �ƴϰ� �ƿ� �ٸ��� �����ߴٴ� �ǹ�

			int32 SwapCharNumber = -1;
			int32 SwapCharFieldNum = -1;
			for (int32 i = 0; i < OwnerController->CurrentParty.Num(); i++)
			{
				if (CharNum == SpawnChars[i]->CharNum)
				{
					SwapCharFieldNum = i;
					SwapCharNumber = SpawnChars[SelectNumber]->CharNum; // �̹� �������ִ� �� ã�Ƽ� ��ü.
					break;
				}
			}


			if (SwapCharNumber != -1)
				SetSpawnUnit(SwapCharFieldNum, SwapCharNumber); // ���� ĳ���� ��ġ ����

			SetSpawnUnit(SelectNumber, CharNum); // �����ֱ�.		

			// �� ���Ⱑ �߿��ϴ�. ��Ʈ�ѷ��� ���� ��Ƽ�� �ִ� �ڸ����� �ľ��ؼ�, �����ϱ����� �ƴ��� �Ǵ��Ѵ�.
			if (OwnerController->CurrentParty[SelectNumber] == CharNum && CharNum != 0)
			{
				// ������ ĳ���͸� �����Ҷ�
				// JRPGSettingPartySlot ������ �����ϱ� ��ư�� Visible �ϱ�
				SettingUI->SetVisibilitySelectButton(false); // �����ϱ� ����
				SettingUI->SetVisibilityOutButton(true); // �����ϱ� �ѱ�.

			}
			else
			{
				SettingUI->SetVisibilityOutButton(false); // �����ϱ� ����.
				SettingUI->SetVisibilitySelectButton(true); // �����ϱ� �ѱ�.
				SettingUI->SetCharNum = CharNum;

				// JRPGSettingPartySlot ������ �����ϱ� ��ư�� Visible �ϱ�.
				// �����ϱ� ��ư�� ������ ����Ʈ ��ü.
			}
		}
		else
		{
			SettingUI->SetVisibilityOutButton(false);
			SettingUI->SetVisibilitySelectButton(false);
		}
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
				{
					SettingPartySlot();			
				}

				SettingUI->SelectFieldNumber = SelectNumber;
				if(OwnerController->CurrentParty.IsValidIndex(SelectNumber))
					SettingUI->SetVisibilityOutButton(true); // �����ϱ� �ѱ�.
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
	SpawnCharacter();
	SettingUI->SetVisibilityOutButton(false);
	SettingUI->SetVisibilitySelectButton(false);
}