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
	QuestNums.Add(QuestNumber);
}

UProjectHGameInstance::UProjectHGameInstance()
{
	FString QuestDataPath = TEXT("DataTable'/Game/PROJECT/BP_CLASS/Blueprints/05_DataBase/DT_QuestData.DT_QuestData'");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_QuestData(*QuestDataPath);
	if (DT_QuestData.Succeeded())
	{
		PQTable = DT_QuestData.Object;
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
	else
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



void UProjectHGameInstance::SetSaveSlot(UQuestComponent* QuestComponent)
{
	if (QuestComponent && QuestSave)
	{
		QuestSave->SaveQuest(QuestComponent->GetQuests(), QuestComponent->GetCurrentID());

	}

}

void UProjectHGameInstance::SetNPCSaveSlot(AQuestNPCBase* NPC)
{
	if (NPC && QuestSave)
	{
		QuestSave->SaveNPC(NPC->NPCName, NPC->NPCQuests, NPC->bQuestSucceed, NPC->bIsQuesting, NPC->bCanAccept, NPC->CanQuestCnt);
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
				{
					PlayerCanQuest[QDB->NPCName].QuestNums.Add(Nums);
				}
				else
				{
					PlayerCanQuest.Add(QDB->NPCName, FCanQuestNums(Nums));
				}
			}
		}
	}
}

FQuestDataBase* UProjectHGameInstance::GetPQData(int32 QuestNumber)
{
	return PQTable->FindRow<FQuestDataBase>(*FString::FromInt(QuestNumber), TEXT(""));
}


/* ���� ������ ����Ʈ�� �������� �� �����ϴ� �Լ�.  �Ϸ������� �����ص��ɵ�.*/
/* ���⼭ NPC �̸����� �޾ƿͼ� PlayerCanQuest TMap�� �����ص��ǳ�, ���ʿ� ����Ʈ�� ������ 
	NPC���� ����Ʈ�� ���������� �̰��ӿ����� �������� �ʾƵ� �ɵ� �ϴ�.*/
void UProjectHGameInstance::AcceptQuestNumber(int32 QuestNumber)
{
	if (PlayerSave)
	{
		if (QuestNums.Contains(QuestNumber))
		{
			QuestNums.RemoveSingle(QuestNumber);
			PlayerSave->SavePlayerQuest(&QuestNums, &FinishedQuests);
			// ���� �и� �ص��ɵ�. QuestNums�� FinishedQuests�� �ٸ������� ������.
		}
	}
}

/* ����Ʈ�� �Ϸ� ���� �� �����ϴ� �Լ� */
void UProjectHGameInstance::FinishedQuestNumber(int32 QuestNumber)
{
	if (PlayerSave)
	{
		FinishedQuests.Add(QuestNumber);
		PlayerSave->SavePlayerQuest(&QuestNums, &FinishedQuests);
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

		return true;
	}
	return false;
}

/* ���� �ػ󵵳� �ؽ�ó ǰ�� ��Ƽ�ٸ���� ���� ������ ini�� �����صξ���.*/
void UProjectHGameInstance::SetDefaultGameSetting()
{
	if (SetDefault())
	{	
		UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), *(RES_COMMAND + R));
		UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), *(AA_COMMAND + FString::FromInt(AA)));
		UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), *(S_COMMAND + FString::FromInt(S)));
		UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), *(T_COMMAND + FString::FromInt(T)));
	}
}

/* ���� �ν��Ͻ����� ��ó���� �ɼǰ��� ���� ���ڿ� �������ִ� �Լ�.*/
void UProjectHGameInstance::GetDefaultGameSetting(FString& Resolution, int32& Anti, int32& ShadowQuality, int32& TextureQuality)
{
	Resolution = R;
	Anti = AA;
	ShadowQuality = S;
	TextureQuality = T;
}


/* �ɼ� ����ÿ� ���� �ν��Ͻ��� �����ϴ� �Լ�. */
void UProjectHGameInstance::GISetGameSetting(FString Resolution, int32 Anti, int32 ShadowQuality, int32 TextureQuality)
{
	R = Resolution;
	AA = Anti;
	S = ShadowQuality;
	T = TextureQuality;

	UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), *(RES_COMMAND + Resolution));
	UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), *(AA_COMMAND + FString::FromInt(Anti)));
	UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), *(S_COMMAND + FString::FromInt(ShadowQuality)));
	UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), *(T_COMMAND + FString::FromInt(TextureQuality)));
}