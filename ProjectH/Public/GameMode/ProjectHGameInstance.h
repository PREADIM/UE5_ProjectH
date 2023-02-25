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
	TSet<int32> QuestNums;

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
	// ĳ���Ϳ� NPC�� ����Ʈ ���¸� �����ϴ� �Լ�. ���� ĳ���Ϳ� NPC���Լ� ȣ���Ѵ�.

	/*-- Quest --*/
	void SetLoadSlot(class UQuestComponent* QuestComponent);
	void SetSaveSlot(class UQuestComponent* QuestComponent);

	bool SetNPCLoadSlot(class AQuestNPCBase* NPC);
	void SetNPCSaveSlot(class AQuestNPCBase* NPC);
	void SetPlayerCanQuest();

	FNPCQuestDataBase* GetNPCQuestData(FString NPCName); // NPC ����Ʈ�� �����´�.
	FQuestDataBase* GetPQData(int32 QuestNumber);
	TArray<FTextNName> GetDialData(EDialougeState DialState, int32 QuestNumber);

	void QuestClearNumber(FString NPCName, int32 QuestNumber); // �ڡ�����Ʈ �Ϸ�� ȣ��.


	/*-- Setting --*/
	bool SetDefault();
	void SetDefaultGameSetting();
	void GetDefaultGameSetting(FString& Resolution, int32& Anti, int32& ShadowQuality, int32& TextureQuality, float& MouseSensitivity);
	void GISetGameSetting(FString Resolution, int32 Anti, int32 ShadowQuality, int32 TextureQuality, float& MouseSensitivity);
	/* ���� �ν��Ͻ��� ���� ������ ���� �����ϸ鼭 ���ÿ� Ŀ�ǵ� ����.*/



public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UQuestSave* QuestSave; // ����Ʈ ���̺�����. NPC�� ����Ʈ�� �÷��̾ �������ִ� ����Ʈ ����.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UPlayerStateSave* PlayerSave; // ĳ���Ϳ� ���� ���̺�����

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TMap<FString, FCanQuestNums> PlayerCanQuest; // NPC�� ���� ��. ����Ʈ �ѹ��� ���������̺��� ������.

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<int32> StartQuestNums; // �ڡڡ� ���� �ʱ⿡ ���డ���� ����Ʈ�� �̸� �����ص�.

	UPROPERTY(VisibleAnywhere)
		TArray<int32> QuestNums; // ���� ������ ����Ʈ �ѹ��� �������ִ�.

	UPROPERTY(VisibleAnywhere)
		TArray<int32> FinishedQuests; // �Ϸ��� ����Ʈ��.

	UPROPERTY(VisibleAnywhere)
		class UDataTable* NPCQBTable; // NPC�� ����Ʈ���� �־����ִ� ���̺��� NPC�� ������ ����Ʈ �������� ���� ���̺�.

	UPROPERTY(VisibleAnywhere)
		class UDataTable* PQTable; // �÷��̾ ������ ����Ʈ���� ���̺�

	UPROPERTY(VisibleAnywhere)
		class UDataTable* DialTable;


	UPROPERTY(VisibleAnywhere)
		TMap<FString, class AQuestNPCBase*> NPCAllPtr; // NPC���� �̸����� �����صд�.
	void SetNPCPtr(FString Name, class AQuestNPCBase* NPC);
	// �ش� �ʿ� �����ϴ� �Լ�,

	UFUNCTION(BlueprintCallable)
		class AQuestNPCBase* GetNPCPtr(FString NPCName);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<FString> ResolutionArr; // �����Ǵ� �ػ� ����.

	/* �� ó�� �ػ󵵵��� �����Ѵ�. */
	UPROPERTY()
		FString R;
	UPROPERTY()
		int32 AA;
	UPROPERTY()
		int32 S;
	UPROPERTY()
		int32 T;
	UPROPERTY()
		float MS;

};
