// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/ProjectHGameInstance.h"
#include "Save/QuestSave.h"
#include "Save/PlayerStateSave.h"
#include "Kismet/GameplayStatics.h"
#include "ActorComponent/QuestComponent/QuestComponent.h"
#include "AI/QuestNPCBase.h"
#include "MainGameSetting.h"
#include "QuestStruct.h"

FCanQuestNums::FCanQuestNums()
{
}

FCanQuestNums::FCanQuestNums(int32 QuestNumber)
{
	QuestNums.Emplace(QuestNumber);
}

UProjectHGameInstance::UProjectHGameInstance()
{
	FString NPCQuestDataPath = TEXT("/Script/Engine.DataTable'/Game/PROJECT/BP_CLASS/Blueprints/05_DataBase/DT_NPCQuestDataBase.DT_NPCQuestDataBase'");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_NPCQuestData(*NPCQuestDataPath);
	if (DT_NPCQuestData.Succeeded())
	{
		NPCQBTable = DT_NPCQuestData.Object;
	}


	FString QuestDataPath = TEXT("/Script/Engine.DataTable'/Game/PROJECT/BP_CLASS/Blueprints/05_DataBase/DT_Quests.DT_Quests'");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_QuestData(*QuestDataPath);
	if (DT_QuestData.Succeeded())
	{
		PQTable = DT_QuestData.Object;
	}

	FString DialogueDataPath = TEXT("/Script/Engine.DataTable'/Game/PROJECT/BP_CLASS/Blueprints/05_DataBase/DT_QuestDials.DT_QuestDials'");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_DialData(*DialogueDataPath);
	if (DT_DialData.Succeeded())
	{
		DialTable = DT_DialData.Object;
	}

	FString LevelTablePath = TEXT("/Script/Engine.DataTable'/Game/PROJECT/BP_CLASS/Blueprints/05_DataBase/DT_LevelPath.DT_LevelPath'");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_LevelPathData(*LevelTablePath);
	if (DT_LevelPathData.Succeeded())
	{
		LevelPathTable = DT_LevelPathData.Object;
	}

	/*FString QuestCinePath = TEXT("/Script/Engine.DataTable'/Game/PROJECT/BP_CLASS/Blueprints/05_DataBase/DT_QuestDials.DT_QuestDials'");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_QuestCine(*QuestCinePath);
	if (DT_QuestCine.Succeeded())
	{
		QuestCinematicTable = DT_QuestCine.Object;
	}*/

}

void UProjectHGameInstance::Init()
{
	Super::Init();

	if(UGameplayStatics::DoesSaveGameExist(UQuestSave::SlotName, 0)) // 퀘스트 슬롯이 있는가?
	{
		QuestSave = Cast<UQuestSave>(UGameplayStatics::LoadGameFromSlot(UQuestSave::SlotName, 0));
	}
	else // 없을경우
	{
		QuestSave = Cast<UQuestSave>(UGameplayStatics::CreateSaveGameObject(UQuestSave::StaticClass()));

	}

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

	TArray<FIntPoint> arr;
	FString AddStr = "x";
	UKismetSystemLibrary::GetSupportedFullscreenResolutions(arr);
	for (auto Res : arr)
	{
		FString X = FString::FromInt(Res.X);
		FString Y = FString::FromInt(Res.Y);
		FString Temp = X + AddStr + Y;
		ResolutionArr.Push(Temp);
	}


	SetDefaultGameSetting();
	/* 지원되는 스크린 저장.*/
}

void UProjectHGameInstance::OpenLevelStart(FString LevelName)
{
	FLevelPath* LevelPath = GetLevelPath(LevelName);
	if (LevelPath)
	{
		UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), LevelPath->Level);
	}	
}

void UProjectHGameInstance::SetNPCPtr(FString Name, AQuestNPCBase* NPC)
{
	if (!NPCAllPtr.Find(Name))
		NPCAllPtr.Add(Name, NPC);
}

AQuestNPCBase* UProjectHGameInstance::GetNPCPtr(FString NPCName)
{
	if (NPCAllPtr.Find(NPCName))
	{
		return NPCAllPtr[NPCName];
	}
	
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
	{
		_DEBUG("Null SetNextQuest");
		return;
	}

	if (Quest->QuestSteps.Num() > 1)
	{
		Quest->QuestSteps.RemoveAt(0);
	}

	QuestSave->SaveSlot();
}

bool UProjectHGameInstance::SetNPCLoadSlot(AQuestNPCBase* NPC)
{
	if (NPC && QuestSave)
	{
		return QuestSave->LoadNPC(NPC);
	}
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

	// 여기서 퀘스트에 해당하는 시네마틱을 가지고있는 데이터 테이블을 만들어서
	// 실행하도록하자.

	/*ALevelSequenceActor* LQActor;
	if (StartSequence)
		SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), StartSequence, FMovieSceneSequencePlaybackSettings(), LQActor);

	if (SequencePlayer)
	{
		BattleUIOnOff(false);
		SequencePlayer->Play();
		return SequencePlayer->GetEndTime().AsSeconds();
	}*/

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
		{
			return Dial->DialogueMap[DialState].Dialogues;
		}
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

bool UProjectHGameInstance::SetDefault()
{
	auto Setting = GetDefault<UMainGameSetting>();
	if (Setting)
	{
		R = Setting->GetResolution();
		AA = Setting->GetAA();
		S = Setting->GetShadowQ();
		T = Setting->GetTextureQ();
		MS = Setting->GetMouseSensitivity();

		return true;
	}
	return false;
}

/* 게임 해상도나 텍스처 품질 안티앨리어싱 같은 세팅을 ini에 저장해두었다.*/
void UProjectHGameInstance::SetDefaultGameSetting()
{
	if (SetDefault()) // SetDefault에서 값들 가져오기.
	{	
		UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), *(RES_COMMAND + R));
		UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), *(AA_COMMAND + FString::FromInt(AA)));
		UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), *(S_COMMAND + FString::FromInt(S)));
		UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), *(T_COMMAND + FString::FromInt(T)));
	}
}

/* 게임 인스턴스안의 맨처음에 옵션값을 참조 인자에 저장해주는 함수.*/
void UProjectHGameInstance::GetDefaultGameSetting(FString& Resolution, int32& Anti, int32& ShadowQuality, int32& TextureQuality, float& MouseSensitivity)
{
	Resolution = R;
	Anti = AA;
	ShadowQuality = S;
	TextureQuality = T;
	MouseSensitivity = MS;
}


/* 옵션 변경시에 게임 인스턴스에 저장하는 함수. */
void UProjectHGameInstance::GISetGameSetting(FString Resolution, int32 Anti, int32 ShadowQuality, int32 TextureQuality, float& MouseSensitivity)
{
	R = Resolution;
	AA = Anti;
	S = ShadowQuality;
	T = TextureQuality;
	MS = MouseSensitivity;

	UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), *(RES_COMMAND + Resolution));
	UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), *(AA_COMMAND + FString::FromInt(Anti)));
	UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), *(S_COMMAND + FString::FromInt(ShadowQuality)));
	UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), *(T_COMMAND + FString::FromInt(TextureQuality)));
}