// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/ProjectHGameInstance.h"
#include "Controller/ProjectH_PC.h"
#include "Save/QuestSave.h"
#include "Save/PlayerStateSave.h"
#include "Kismet/GameplayStatics.h"
#include "ActorComponent/QuestComponent/QuestComponent.h"
#include "AI/QuestNPCBase.h"
#include "MainGameSetting.h"
#include "QuestStruct.h"
#include "Blueprint/UserWidget.h"
#include "Special/PlaySequenceActor.h"
#include <LevelSequencePlayer.h>
#include <LevelSequenceActor.h>
#include "GameFramework/GameUserSettings.h"
#include "UI/Custom/LoadingScreenWidget.h"

FCanQuestNums::FCanQuestNums()
{
}

FCanQuestNums::FCanQuestNums(int32 QuestNumber)
{
	QuestNums.Emplace(QuestNumber);
}

UProjectHGameInstance::UProjectHGameInstance()
{
	/*FString NPCQuestDataPath = TEXT("DataTable'/Game/PROJECT/BP_CLASS/Blueprints/05_DataBase/DT_NPCQuestDataBase.DT_NPCQuestDataBase'");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_NPCQuestData(*NPCQuestDataPath);
	if (DT_NPCQuestData.Succeeded())
		NPCQBTable = DT_NPCQuestData.Object;*/


	FString QuestDataPath = TEXT("DataTable'/Game/PROJECT/BP_CLASS/Blueprints/05_DataBase/DT_Quests.DT_Quests'");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_QuestData(*QuestDataPath);
	if (DT_QuestData.Succeeded())
		PQTable = DT_QuestData.Object;

	FString DialogueDataPath = TEXT("DataTable'/Game/PROJECT/BP_CLASS/Blueprints/05_DataBase/DT_QuestDials.DT_QuestDials'");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_DialData(*DialogueDataPath);
	if (DT_DialData.Succeeded())
		DialTable = DT_DialData.Object;

	FString LevelTablePath = TEXT("DataTable'/Game/PROJECT/BP_CLASS/Blueprints/05_DataBase/DT_LevelPath.DT_LevelPath'");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_LevelPathData(*LevelTablePath);
	if (DT_LevelPathData.Succeeded())
		LevelPathTable = DT_LevelPathData.Object;

	// 왜 안되는가?
	/*FString SequenceActorPath = TEXT("/Game/PROJECT/BP_CLASS/Blueprints/05_DataBase/DT_SequenceActorTable.DT_SequenceActorTable");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_SequenceActor(*SequenceActorPath);
	if (DT_SequenceActor.Succeeded())
	{
		SequenceActorTable = DT_SequenceActor.Object;
	}*/

}

void UProjectHGameInstance::Init()
{
	Super::Init();

	UserSettings = GEngine->GetGameUserSettings();
	if (UserSettings)
	{
		UserSettings->SetGlobalIlluminationQuality(0);
		UserSettings->SetReflectionQuality(2);
		UserSettings->SetFoliageQuality(1);
	}

	if(UGameplayStatics::DoesSaveGameExist(UQuestSave::SlotName, 0)) // 퀘스트 슬롯이 있는가?
		QuestSave = Cast<UQuestSave>(UGameplayStatics::LoadGameFromSlot(UQuestSave::SlotName, 0));
	else // 없을경우
		QuestSave = Cast<UQuestSave>(UGameplayStatics::CreateSaveGameObject(UQuestSave::StaticClass()));

	/* 이 배열로 통해 어떤맵이든 간에 해당 배열에 값을 저장만 하면 퀘스트를 다음으로 넘길수 있다. */
	if (QuestSave->IsSaveFromOtherMapSuccessQuest())
		RunTimeSuccessQuestNumberQueue = QuestSave->LoadFromOtherMapSuccessQuest();

	if (UGameplayStatics::DoesSaveGameExist(UPlayerStateSave::SlotName, 0))
	{
		PlayerSave = Cast<UPlayerStateSave>(UGameplayStatics::LoadGameFromSlot(UPlayerStateSave::SlotName, 0));
		if(PlayerSave)
			SetPlayerCanQuest(); // 이미 저장된 퀘스트 목록들을 가져온다.
	}
	else // 플레이어 퀘스트가 없는경우.
	{
		PlayerSave = Cast<UPlayerStateSave>(UGameplayStatics::CreateSaveGameObject(UPlayerStateSave::StaticClass()));
		if (PlayerSave)
		{
			QuestNums = StartQuestNums;
			PlayerSave->SavePlayerQuest(&QuestNums, &FinishedQuests);
			SetPlayerCanQuest();
			// 처음 생성하면 기초적으로 가지고 있어야할 목록을 저장한다.
		}
	}

	/* 지원되는 스크린 저장.*/
	FString AddStr = "x";
	UKismetSystemLibrary::GetSupportedFullscreenResolutions(ResArr);
	ResolutionArr.Empty();
	for (auto Res : ResArr)
	{
		FString X = FString::FromInt(Res.X);
		FString Y = FString::FromInt(Res.Y);
		FString Temp = X + AddStr + Y;
		ResolutionArr.Push(Temp);
	}


	SetDefaultGameSetting();
	
}

/* 오픈할 레벨 이름과 화면이 어두워지는 시퀀스를 사용할 것인지. */
void UProjectHGameInstance::OpenLevelStart(FString LevelName, bool bPlaySequence, class APlayerControllerBase* PCBase)
{
	if (PCBase != nullptr)
	{
		PCBase->OnHiddenWidget.Broadcast();
		PCBase->DisableInput(PCBase);
		PCBase->SetShowMouseCursor(false);
		PCBase->SetInputMode(FInputModeGameOnly());
	}
		
	if (bOpeningLevel)
		return;

	FLevelPath* LevelPathTemp = GetLevelPath(LevelName);
	if (!LevelPathTemp)
		return;

	/* 예외 */
	if (!BP_LoadingScreen)
	{
		LodeMap();
		return;
	}

	bOpeningLevel = true; /* 중복 방지 */
	LevelPath = *LevelPathTemp;	

	if(!LoadingScreen)
		LoadingScreen = CreateWidget<ULoadingScreenWidget>(GetWorld(), BP_LoadingScreen);

	LoadingScreen->SetScreenImage(LevelName);
	
	if (bPlaySequence)
	{
		SequencePlayer = nullptr;
		ALevelSequenceActor* LQActor;
		if (OpenLevelSequence)
			SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), OpenLevelSequence, FMovieSceneSequencePlaybackSettings(), LQActor);

		float EndTime = 1.f;
		if (SequencePlayer)
		{
			SequencePlayer->Play();
			EndTime = SequencePlayer->GetEndTime().AsSeconds();

			FTimerHandle Handle;
			GetWorld()->GetTimerManager().SetTimer(Handle, this, &UProjectHGameInstance::OpenLevelSepuenceEnd, EndTime, false);
		}
		else
		{
			/* 예외 */
			OpenLevelSepuenceEnd();
		}
	}
	else
		OpenLevelSepuenceEnd();
		
}

void UProjectHGameInstance::OpenLevelSepuenceEnd()
{
	LoadingScreen->AddToViewport();
	LoadPackageAsync(LevelPath.Level.ToSoftObjectPath().ToString(), FLoadPackageAsyncDelegate::CreateLambda([=](const FName& PackageName, UPackage* Package, EAsyncLoadingResult::Type Result)
		{
			if (Result == EAsyncLoadingResult::Succeeded)
				AsyncLodedMap();
		}), 0, PKG_ContainsMap);
}

void UProjectHGameInstance::AsyncLodedMap()
{
	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, this, &UProjectHGameInstance::LodeMap, 1.5f, false);
}

void UProjectHGameInstance::LodeMap()
{
	bOpeningLevel = false;
	UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), LevelPath.Level, true);
}

APlaySequenceActor* UProjectHGameInstance::PlaySequence(int32 SequenceNumber, APlayerControllerBase* Controller)
{
	// PlaySequenceActor를 소환하면 해당 액터에 바인딩된 함수들이 실행될 것이다.
	FSequenceActorTable* SAStruct = GetSequenceActor(SequenceNumber);
	if (SAStruct)
	{
		if (SAStruct->BP_SequenceActor)
		{
			APlaySequenceActor* SA = GetWorld()->SpawnActorDeferred<APlaySequenceActor>(SAStruct->BP_SequenceActor, FTransform());
			if (SA)
			{
				SA->GI = this;
				SA->PCBase = Controller;
			}
			SA->FinishSpawning(FTransform());

			return SA;
		}
	}

	return nullptr;
}

void UProjectHGameInstance::SetNPCPtr(FString Name, AQuestNPCBase* NPC)
{
	if (!NPCAllPtr.Find(Name))
		NPCAllPtr.Emplace(Name, NPC);
	else
		NPCAllPtr[Name] = NPC;
	/* 레벨 이동하고 나서 새롭게 저장할 때 */
}

AQuestNPCBase* UProjectHGameInstance::GetNPCPtr(FString NPCName)
{
	if (NPCAllPtr.Find(NPCName))
		return NPCAllPtr[NPCName];

	return nullptr;
}


void UProjectHGameInstance::SetLoadSlot(class UQuestComponent* QuestComponent)
{
	if (QuestComponent && QuestSave)
	{
		QuestSave->LoadQuest(QuestComponent);
	}
}


void UProjectHGameInstance::SetSaveSlot(UQuestComponent* QuestComponent)
{
	if (QuestComponent && QuestSave)
	{
		QuestSave->SaveQuest(QuestComponent->GetQuests(), QuestComponent->GetCurrentID());
	}
}

void UProjectHGameInstance::SetNextQuest(int32 QuestNumber)
{
	FQuestStruct* Quest = QuestSave->GetQuests(QuestNumber);
	if (!Quest)
		return;

	if (Quest->QuestSteps.Num() > 1)
		Quest->QuestSteps.RemoveAt(0);

	QuestSave->SaveSlot();
}

bool UProjectHGameInstance::SetNPCLoadSlot(AQuestNPCBase* NPC)
{
	if (NPC && QuestSave)
		return QuestSave->LoadNPC(NPC);

	return false;
}

void UProjectHGameInstance::SetNPCSaveSlot(AQuestNPCBase* NPC)
{
	if (NPC && QuestSave)
	{
		QuestSave->SaveNPC(NPC->NPCName, NPC->QuestingNums, NPC->SucceedQuestsNums, NPC->EndedQuestsNums);
	}
}


/* 세이브파일에서 가져온 QuestNums를 통해 NPC이름과 퀘스트 이름을 가져온다.*/
void UProjectHGameInstance::SetPlayerCanQuest()
{
	PlayerSave->LoadPlayerQuest(&QuestNums, &FinishedQuests);
	if (QuestNums.Num() > 0)
	{
		for (int32 Nums : QuestNums)
		{
			FQuestDataBase* QDB = GetPQData(Nums);
			if (QDB)
			{
				if (PlayerCanQuest.Find(QDB->NPCName))
					PlayerCanQuest[QDB->NPCName].QuestNums.Emplace(Nums);
				else
					PlayerCanQuest.Emplace(QDB->NPCName, FCanQuestNums(Nums));
			}
		}
	}
}

// 가능해진 퀘스트를 추가한다. (런타임 중 실행하는 함수.)
void UProjectHGameInstance::AddCanQuest(int32 QuestNumber)
{
	QuestNums.Emplace(QuestNumber);

	// 퀘스트 전체리스트에서 해당 NPC이름을 찾아와서 대입.
	FQuestDataBase* QDB = GetPQData(QuestNumber);
	if (QDB)
	{
		if (PlayerCanQuest.Find(QDB->NPCName))
			PlayerCanQuest[QDB->NPCName].QuestNums.Emplace(QuestNumber);
		else
			PlayerCanQuest.Emplace(QDB->NPCName, FCanQuestNums(QuestNumber));
	}

	PlayerSave->SavePlayerQuest(&QuestNums, &FinishedQuests);
}



FNPCQuestDataBase* UProjectHGameInstance::GetNPCQuestData(FString NPCName)
{
	return NPCQBTable->FindRow<FNPCQuestDataBase>(*NPCName, TEXT(""));
}

FQuestDataBase* UProjectHGameInstance::GetPQData(int32 QuestNumber)
{
	return PQTable->FindRow<FQuestDataBase>(*FString::FromInt(QuestNumber), TEXT(""));
}

TArray<FTextNName> UProjectHGameInstance::GetDialData(EDialougeState DialState, int32 QuestNumber)
{
	FDialogueStruct* Dial = DialTable->FindRow<FDialogueStruct>(*FString::FromInt(QuestNumber), TEXT(""));
	if (Dial)
	{
		if (Dial->DialogueMap.Find(DialState))
			return Dial->DialogueMap[DialState].Dialogues;
	}

	return TArray<FTextNName>();
}

FLevelPath* UProjectHGameInstance::GetLevelPath(FString LevelName)
{
	if (LevelPathTable)
	{
		return LevelPathTable->FindRow<FLevelPath>(*LevelName, TEXT(""));
	}
	return nullptr;
}

FSequenceActorTable* UProjectHGameInstance::GetSequenceActor(int32 SequenceNumber)
{
	if (SequenceActorTable)
	{
		return SequenceActorTable->FindRow<FSequenceActorTable>(*FString::FromInt(SequenceNumber), TEXT(""));
	}
	return nullptr;
}


/* 퀘스트를 완료했을때 제거하도록 하자.*/
void UProjectHGameInstance::QuestClearNumber(FString NPCName, int32 QuestNumber)
{
	if (PlayerSave)
	{
		if (QuestNums.Contains(QuestNumber))
		{
			PlayerCanQuest[NPCName].QuestNums.Remove(QuestNumber);
			QuestNums.RemoveSingle(QuestNumber);
			FinishedQuests.Emplace(QuestNumber);
			PlayerSave->SavePlayerQuest(&QuestNums, &FinishedQuests);
		}
	}
}


/* 실시간으로 메인레벨 퀘스트 추가 시키기 (다른레벨 x) */
void UProjectHGameInstance::AddQuestRunTime(FString NPCName, int32 QuestNumber, AProjectH_PC* PlayerController)
{
	AQuestNPCBase* NPC = GetNPCPtr(NPCName);
	if (!NPC)
		return;

	UQuestComponent* QuestComponent = PlayerController->GetQuestComponent();
	if (!QuestComponent)
		return;

	FNPCQuest* NewQuest = NPC->AddQuestRunTime(QuestNumber);
	if (NewQuest)
	{
		QuestComponent->AddQuest(*NewQuest);
		SetSaveSlot(QuestComponent);
	}

	PlayerController->SetQuestCollisionSetup();
}


/* 다른 테마에서 메인레벨 퀘스트 완료 시키기 */
/* 해당 배열에 들어가있는 넘버들을 기반으로 메인 컨트롤러가 맨처음 Begin할때 ComplteStep을 해준다.*/
void UProjectHGameInstance::SuccessQuestRunTime(int32 QuestNumber)
{
	RunTimeSuccessQuestNumberQueue.Emplace(QuestNumber);
	QuestSave->SaveFromOtherMapSuccessQuest(&RunTimeSuccessQuestNumberQueue);
}


bool UProjectHGameInstance::SetDefault()
{
	if (!UserSettings)
		UserSettings = GEngine->GetGameUserSettings();

	auto Setting = GetDefault<UMainGameSetting>();
	if (Setting)
	{
		ResIndex = Setting->GetResIndex();
		if (!ResArr.IsValidIndex(ResIndex))
			ResIndex = ResArr.Num() - 1;

		AA = UserSettings->GetAntiAliasingQuality();
		S = UserSettings->GetShadowQuality();
		T = UserSettings->GetTextureQuality();
		MS = Setting->GetMouseSensitivity();
		MSound = Setting->GetMaster();

		return true;
	}
	return false;

}

/* 게임 해상도나 텍스처 품질 안티앨리어싱 같은 세팅을 ini에 저장해두었다.*/
void UProjectHGameInstance::SetDefaultGameSetting()
{
	if (SetDefault()) // SetDefault에서 값들 가져오기.
	{	
		UserSettings->SetScreenResolution(ResArr[ResIndex]);
		UserSettings->ApplyResolutionSettings(false);

		UserSettings->SetAntiAliasingQuality(AA);

		UserSettings->SetShadowQuality(S);
		UserSettings->SetPostProcessingQuality(S);


		UserSettings->SetTextureQuality(T);
		UserSettings->SetVisualEffectQuality(T);
		UserSettings->SetShadingQuality(T);
		

		UserSettings->ApplySettings(false);
	}
}

/* 게임 인스턴스안의 맨처음에 옵션값을 참조 인자에 저장해주는 함수.*/
void UProjectHGameInstance::GetDefaultGameSetting(int32* ResolutionIndex, int32* Anti, int32* ShadowQuality, int32* TextureQuality, float* MouseSensitivity, float* MasterSound)
{
	*ResolutionIndex = ResIndex;
	*Anti = AA;
	*ShadowQuality = S;
	*TextureQuality = T;
	*MouseSensitivity = MS;
	*MasterSound = MSound;
}


/* 옵션 변경시에 게임 인스턴스에 저장하는 함수. */
void UProjectHGameInstance::GISetGameSetting(int32 ResolutionIndex, int32 Anti, int32 ShadowQuality, int32 TextureQuality, float MouseSensitivity, float MasterSound)
{
	if (ResIndex != ResolutionIndex)
		UserSettings->SetFullscreenMode(EWindowMode::Windowed);

	ResIndex = ResolutionIndex;
	AA = Anti;
	S = ShadowQuality;
	T = TextureQuality;
	MS = MouseSensitivity;
	MSound = MasterSound;

	UserSettings->SetScreenResolution(ResArr[ResIndex]);
	UserSettings->ApplyResolutionSettings(false);

	UserSettings->SetAntiAliasingQuality(AA);

	UserSettings->SetShadowQuality(S);
	UserSettings->SetPostProcessingQuality(S);


	UserSettings->SetTextureQuality(T);
	UserSettings->SetVisualEffectQuality(T);
	UserSettings->SetShadingQuality(T);

	UserSettings->ApplySettings(false);
}


void UProjectHGameInstance::SetDontPlayEnding()
{
	bDontPlayEnding = true;
}


bool UProjectHGameInstance::CanPlayEnding()
{
	return !bDontPlayEnding;
}