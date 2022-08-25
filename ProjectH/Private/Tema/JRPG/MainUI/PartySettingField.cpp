// Fill out your copyright notice in the Description page of Project Settings.


#include "Tema/JRPG/MainUI/PartySettingField.h"
#include "Tema/JRPG/JRPGPlayerController.h"
#include "Tema/JRPG/MainUI/JRPGSettingPartyIcon.h"
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


	RootComponent = Root;
	Camera->SetupAttachment(Root);

	Unit1->SetupAttachment(Root);
	Unit2->SetupAttachment(Root);
	Unit3->SetupAttachment(Root);


	TEnumAsByte<EObjectTypeQuery> OT = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel8);
	ObjectTypes.Add(OT);

	CurrentLocation = FVector(239.f, -20.f,143.f);
	TargetLocation = FVector(239.f, -20.f, 143.f);

	SpawnChars.Init(nullptr, 3);
	bOpenUI = false;

	CurrentPartyIcons.Init(nullptr, 3);
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
	for(int32 i = 0; i < 3; i++)
	{
		if (SpawnChars[i] != nullptr)
		{
			SpawnChars[i]->Destroy();
		}
	}


	CurrentParty = OwnerController->CurrentParty;

	for (int32 i = 0; i < 3; i++)
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


/* 파티셋팅에서 파티를 변경했을때 해당 칸만 실시간으로 새로 스폰하는 함수.*/
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
	}

	if (SpawnChars[Number] != nullptr)
	{
		SpawnChars[Number]->Destroy();
		SpawnChars[Number] = GM->GetCharacterSpawn(CharNum, UnitLocation); // 선택한 캐릭터 미리 보기
		// 컨트롤러의 CurrentParty는 건들지않음.
	}
	else
	{
		SpawnChars[Number] = GM->GetCharacterSpawn(CharNum, UnitLocation);
	}
	

}


// 파티 캐릭터의 목록이 변경되었으므로, 다시 설정
// false는 그 아이콘이 다시 기본 색으로 변하게하는것. true는 그 아이콘 색이 변하는 것.
void APartySettingField::SetPartyList(int32 CharNum, UJRPGSettingPartyIcon* OwnerIcon)
{
	if (SettingUI)
	{
		// 아예 새로 추가하는 로직
		if (!OwnerController->CurrentParty.IsValidIndex(SelectNumber))
		{

			bool bCan = true; // 이미 파티에 있으면 할필요 x
			for (int32 i = 0; i < OwnerController->CurrentParty.Num(); i++)
			{
				if (CharNum == SpawnChars[i]->CharNum)
				{
					bCan = false; // 이미 있으므로 하지 말것
					SettingUI->PlayWarningText();
					//여기서 이미 존재한다고 경고창 띄우기.
					// OwnerController->LastWidget에 넣기.
					break;
				}
			}

			if (bCan)
			{
				SetSpawnUnit(SelectNumber, CharNum);
				SettingUI->SetCharNum = CharNum;
				SettingUI->SetPartyChange(); // 새롭게 추가하고 바로 나가기.
			}	

			return;
		}

		// 위의 조건문은 아예 새로 추가하므로 아래 로직을 실행할 필요가 x

		if (SpawnChars[SelectNumber]->CharNum == CharNum && CharNum != 0) // 같은걸 선택하면 애초에 해제하기만 떠야함. 스폰 할 필요x
		{
			// ★ 여기가 중요하다. 컨트롤러의 원래 파티에 있던 자리인지 파악해서, 해제하기인지 아닌지 판단한다.
			if (OwnerController->CurrentParty[SelectNumber] == CharNum && CharNum != 0)
			{
				// 선택한 캐릭터를 해제할때
				// JRPGSettingPartySlot 위젯의 해제하기 버튼을 Visible 하기
				SettingUI->SetVisibilitySelectButton(false); // 선택하기 끄기
				SettingUI->SetVisibilityOutButton(true); // 해제하기 켜기.

			}
			else
			{
				SettingUI->SetVisibilityOutButton(false); // 해제하기 끄기.
				SettingUI->SetVisibilitySelectButton(true); // 선택하기 켜기.
				SettingUI->SetCharNum = CharNum;

				// JRPGSettingPartySlot 위젯의 선택하기 버튼을 Visible 하기.
				// 선택하기 버튼을 누르면 리스트 교체.
			}
			// 스폰할 필요x 아래에 따로 또 있는 이유는, 
			// 다른걸 눌렀다가, 다시 원래 편성된 캐릭터를 눌렀을때 해제하기가 뜨게하기 위함이다.
		}		
		else if (SpawnChars[SelectNumber]->CharNum != CharNum && CharNum != 0)
		{
			// CurrentParty와 다르고, 스폰 캐릭터와도 다르다는 얘기는, 기존에 선택된 캐릭터도 아니고 아예 다른걸 선택했다는 의미

			int32 SwapCharNumber = -1;
			SwapCharFieldNum = -1;
			for (int32 i = 0; i < OwnerController->CurrentParty.Num(); i++)
			{
				if (CharNum == SpawnChars[i]->CharNum)
				{
					SwapCharFieldNum = i;
					SwapCharNumber = SpawnChars[SelectNumber]->CharNum; // 이미 가지고있는 곳 찾아서 교체.
					break;
				}
			}


			if (SwapCharNumber != -1 && SelectNumber != SwapCharNumber)
			{		
				SetSpawnUnit(SwapCharFieldNum, SwapCharNumber); // 기존 캐릭터 위치 변경		
			}
			else if(SwapCharNumber == -1 || SelectNumber == SwapCharNumber)// -1인데도 불구하고 이 로직이 실행된다는 것은, 파티에는없는데 캐릭터를 교체 한다는 뜻.
			{
				SwapIconColor();
				CurrentPartyIcons[SelectNumber] = OwnerIcon; // 새로운 아이콘이 자리 잡기.
			}

			SetSpawnUnit(SelectNumber, CharNum); // 보여주기.		

			// ★ 여기가 중요하다. 컨트롤러의 원래 파티에 있던 자리인지 파악해서, 해제하기인지 아닌지 판단한다.
			if (OwnerController->CurrentParty[SelectNumber] == CharNum && CharNum != 0)
			{
				// 선택한 캐릭터를 해제할때
				// JRPGSettingPartySlot 위젯의 해제하기 버튼을 Visible 하기
				SettingUI->SetVisibilitySelectButton(false); // 선택하기 끄기
				SettingUI->SetVisibilityOutButton(true); // 해제하기 켜기.

			}
			else
			{
				SettingUI->SetVisibilityOutButton(false); // 해제하기 끄기.
				SettingUI->SetVisibilitySelectButton(true); // 선택하기 켜기.
				SettingUI->SetCharNum = CharNum;

				// JRPGSettingPartySlot 위젯의 선택하기 버튼을 Visible 하기.
				// 선택하기 버튼을 누르면 리스트 교체.
			}
		}
		else
		{
			SettingUI->SetVisibilityOutButton(false);
			SettingUI->SetVisibilitySelectButton(false);
		}
	}
	
}


// 파티 캐릭터의 목록이 변경되었으므로, 캐릭터 위치 재설정
// ★★ 선택하기나 해제하기 눌렀을때 실행되게한다.
void APartySettingField::SetPartyChar()
{
	SpawnCharacter(); // 컨트롤러의 CurrentParty를 다시 새롭게 받아오면서 새로 스폰하기.
	ResomeUI();
}

void APartySettingField::SwapIconColor()
{
	if (SwapCharFieldNum == -1)
	{
		CurrentPartyIcons[SelectNumber]->SetSelectColor(false);
	}
}


void APartySettingField::LMB()
{
	if (bOpenUI)
		return; // UI 켜져있으면 실행x

	FHitResult HitResult;

	bool bHit = OwnerController->GetHitResultUnderCursorForObjects(ObjectTypes, false, HitResult);
	if (bHit)
	{
		class AJRPGUnit* LastUnit = Cast<class AJRPGUnit>(HitResult.GetActor());
		if (LastUnit)
		{
			_DEBUG("%s", *LastUnit->GetName());

			for (int32 i = 0; i < 4; i++)
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
					}

					SelectNumber = i; // 이걸 기억하고있어야 캐릭터 아이콘을 선택했을때 해당 칸을 변경가능
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
					SettingUI->SetVisibilityOutButton(true); // 해제하기 켜기.
			}	
		}
	}
}

void APartySettingField::NextChar(int32 Number)
{
	// 문제점, 옆으로 넘겼을때 SettingUI에도 당연히 SelectFieldNumber 전해줘야함.
	// 해제하기 편성하기가 해당 칸에 이동하면 떠야함.
	// 때문에 그냥 원신처럼 옆으로 이동못하게 하는것이 나을수도 있다.
	int32 MaxCnt = 3;

	int32 i = SelectNumber + Number; // +1 or -1

	if (i < 0 || i > MaxCnt)
		return;
	// CurrentParty와 >=으로 한 이유는 한칸 옆이 빈칸이라면 그 해당 칸까지는 가야하기 때문.

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
	}

	SelectNumber = i; // 이걸 기억하고있어야 캐릭터 아이콘을 선택했을때 해당 칸을 변경가능	
	SettingUI->SelectFieldNumber = SelectNumber;
	// 아예 SettingUI에서 SelectFieldNumber를 뺴고 OnwerField->SelctNumber 에서 직접 가져와서 사용하면 해결가능.
	
}


// 파티 변경하는 UI 띄우기.
void APartySettingField::SettingPartySlot()
{
	if (SettingUI)
	{
		SettingUI->SetList();
		SettingUI->AddToViewport();
		bOpenUI = true;
	}
}


//SettingSlotUI에서 다시 맨처음 파티 UI 설정으로 돌아가기.
void APartySettingField::ResomeUI()
{
	PartySettingUI->SetVisibility(ESlateVisibility::Visible);
	TargetLocation = ZeroLocation;
	SpawnCharacter();
	SettingUI->SetVisibilityOutButton(false);
	SettingUI->SetVisibilitySelectButton(false);
	bOpenUI = false;
}