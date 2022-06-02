// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Engine/GameInstance.h"
#include "QuestDataBase.h"
#include "Dialogue/TextNAnim.h"
#include "Dialogue/DialogueStruct.h"
#include "Engine/DataTable.h"
#include "ProjectHGameInstance.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FCanQuestNums
{
	GENERATED_USTRUCT_BODY()
public:
	TArray<int32> QuestNums;

	FCanQuestNums();
	FCanQuestNums(int32 QuestNumber);

};


UCLASS()
class PROJECTH_API UProjectHGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UProjectHGameInstance();

	virtual void Init() override;
	// 캐릭터와 NPC의 퀘스트 상태를 저장하는 함수. 각각 캐릭터와 NPC에게서 호출한다.

	/*-- Quest --*/
	void SetSaveSlot(class UQuestComponent* QuestComponent);
	void SetNPCSaveSlot(class AQuestNPCBase* NPC);
	void SetPlayerCanQuest();

	FQuestDataBase* GetPQData(int32 QuestNumber);
	TArray<FTextNAnim> GetDialData(int32 QuestNumber);


	void AcceptQuestNumber(int32 QuestNumber); // 진행 가능한 퀘스트를 수락했을 때

	void FinishedQuestNumber(int32 QuestNumber); /* 퀘스트를 완료 했을 때 변경하는 함수 */


	/*-- Setting --*/
	bool SetDefault();
	void SetDefaultGameSetting();
	void GetDefaultGameSetting(FString& Resolution, int32& Anti, int32& ShadowQuality, int32& TextureQuality);
	void GISetGameSetting(FString Resolution, int32 Anti, int32 ShadowQuality, int32 TextureQuality);
	/* 게임 인스턴스에 새로 설정된 셋팅 저장하면서 동시에 커맨드 실행.*/



public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UQuestSave* QuestSave; // 퀘스트 세이브파일. NPC의 퀘스트나 플레이어가 가지고있는 퀘스트 저장.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UPlayerStateSave* PlayerSave; // 캐릭터에 관한 세이브파일


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TMap<FString, FCanQuestNums> PlayerCanQuest; // NPC와 비교할 것. 퀘스트 넘버로 데이터테이블에서 가져옴.

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<int32> StartQuestNums; // ★★★ 실행 초기에 실행가능한 퀘스트를 미리 지정해둠.

	UPROPERTY(VisibleAnywhere)
		TArray<int32> QuestNums; // 진행 가능한 퀘스트 넘버로 가지고있다.
	UPROPERTY(VisibleAnywhere)
		TArray<int32> FinishedQuests; // 완료한 퀘스트들.

	UPROPERTY(VisibleAnywhere)
		class UDataTable* PQTable;

	UPROPERTY(VisibleAnywhere)
		class UDataTable* DialTable;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<FString> ResolutionArr; // 지원되는 해상도 저장.

	/* 맨 처음 해상도들을 저장한다. */
	UPROPERTY()
		FString R;
	UPROPERTY()
		int32 AA;
	UPROPERTY()
		int32 S;
	UPROPERTY()
		int32 T;





	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TMap<FString, class AQuestNPCBase> AllNPC;
	void SaveAllNPC();
	void LoadAllNPC(); */

};
