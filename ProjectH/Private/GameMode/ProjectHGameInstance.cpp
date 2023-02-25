// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/ProjectHGameInstance.h"
#include "Save/QuestSave.h"
#include "Save/PlayerStateSave.h"
#include "Kismet/GameplayStatics.h"
#include "ActorComponent/QuestComponent/QuestComponent.h"
#include "AI/QuestNPCBase.h"
#include "MainGameSetting.h"

FCanQuestNums::FCanQuestNums()
{
}

FCanQuestNums::FCanQuestNums(int32 QuestNumber)
{
	QuestNums.Emplace(QuestNumber);
}

UProjectHGameInstance::UProjectHGameInstance()
{
	FString NPCQuestDataPath = TEXT("/Script/Engine.DataTable'/Game/PROJECT/BP_CLASS/Blueprints/05_DataBase/DT_NPCQuestDataBase'");
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

	FString DialogueDataPath = TEXT("/Script/Engine.DataTable'/Game/PROJECT/BP_CLASS/Blueprints/05_DataBase/DT_QuestDialogues'");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_DialData(*DialogueDataPath);
	if (DT_DialData.Succeeded())
	{
		DialTable = DT_DialData.Object;
	}

}

void UProjectHGameInstance::Init()
{
	Super::Init();

	if(UGameplayStatics::DoesSaveGameExist(UQuestSave::SlotName, 0)) // ����Ʈ ������ �ִ°�?
	{
		QuestSave = Cast<UQuestSave>(UGameplayStatics::LoadGameFromSlot(UQuestSave::SlotName, 0));
	}
	else // �������
	{
		QuestSave = Cast<UQuestSave>(UGameplayStatics::CreateSaveGameObject(UQuestSave::StaticClass()));

	}

	if (UGameplayStatics::DoesSaveGameExist(UPlayerStateSave::SlotName, 0))
	{
		PlayerSave = Cast<UPlayerStateSave>(UGameplayStatics::LoadGameFromSlot(UPlayerStateSave::SlotName, 0));
		if(PlayerSave)
			SetPlayerCanQuest(); // �̹� ����� ����Ʈ ��ϵ��� �����´�.
	}
	else // �÷��̾� ����Ʈ�� ���°��.
	{
		PlayerSave = Cast<UPlayerStateSave>(UGameplayStatics::CreateSaveGameObject(UPlayerStateSave::StaticClass()));
		if (PlayerSave)
		{
			QuestNums = StartQuestNums;
			PlayerSave->SavePlayerQuest(&QuestNums, &FinishedQuests);
			SetPlayerCanQuest();
			// ó�� �����ϸ� ���������� ������ �־���� ����� �����Ѵ�.
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
	/* �����Ǵ� ��ũ�� ����.*/
}

void UProjectHGameInstance::SetNPCPtr(FString Name, AQuestNPCBase* NPC)
{
	if (!NPCAllPtr.Find(Name))
		NPCAllPtr.Add(Name, NPC);
}

AQuestNPCBase* UProjectHGameInstance::GetNPCPtr(FString NPCName)
{
	return *NPCAllPtr.Find(NPCName);
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


/* ���̺����Ͽ��� ������ QuestNums�� ���� NPC�̸��� ����Ʈ �̸��� �����´�.*/
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
					PlayerCanQuest[QDB->NPCName].QuestNums.Add(Nums);
				else
					PlayerCanQuest.Add(QDB->NPCName, FCanQuestNums(Nums));
			}
		}
	}
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


/* ����Ʈ�� �Ϸ������� �����ϵ��� ����.*/
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

/* ���� �ػ󵵳� �ؽ�ó ǰ�� ��Ƽ�ٸ���� ���� ������ ini�� �����صξ���.*/
void UProjectHGameInstance::SetDefaultGameSetting()
{
	if (SetDefault()) // SetDefault���� ���� ��������.
	{	
		UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), *(RES_COMMAND + R));
		UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), *(AA_COMMAND + FString::FromInt(AA)));
		UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), *(S_COMMAND + FString::FromInt(S)));
		UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), *(T_COMMAND + FString::FromInt(T)));
	}
}

/* ���� �ν��Ͻ����� ��ó���� �ɼǰ��� ���� ���ڿ� �������ִ� �Լ�.*/
void UProjectHGameInstance::GetDefaultGameSetting(FString& Resolution, int32& Anti, int32& ShadowQuality, int32& TextureQuality, float& MouseSensitivity)
{
	Resolution = R;
	Anti = AA;
	ShadowQuality = S;
	TextureQuality = T;
	MouseSensitivity = MS;
}


/* �ɼ� ����ÿ� ���� �ν��Ͻ��� �����ϴ� �Լ�. */
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