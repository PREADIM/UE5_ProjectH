// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Components/ActorComponent.h"
#include "QuestStruct.h"
#include "QuestStep.h"
#include "NPCQuest.h"
#include "QuestComponent.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnUpdateQuestList); // ����Ʈ�� Ŭ�����ϰų� ���� �߰��Ҷ�
DECLARE_MULTICAST_DELEGATE_OneParam(FOnUpdateDescription, int32); // ����Ʈ �Ϸ� ������


USTRUCT(BlueprintType)
struct FActiveQuest
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FString QuestName;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int32 QuestNumber;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FString OwnerNPCName;

	FActiveQuest();
	void Clear();
	void SetActiveQuest(FQuestStruct& Quest);
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTH_API UQuestComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UQuestComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


public:
	/* ------------------
		public Value
	--------------------*/
	
	FOnUpdateQuestList OnUpdateQuestList; // ����Ʈ����Ʈ��������Ʈ �Ҷ� ������ ��������Ʈ.
	FOnUpdateDescription OnUpdateDescription; // ����Ʈ �� �ܰ� Ŭ����� ȣ���� ��������Ʈ.

	UPROPERTY(VisibleAnywhere)
		class AProjectHCharacter* OwnerCharacter;

	UPROPERTY(VisibleAnywhere)
		class AProjectH_PC* OwnerController;

	UPROPERTY(VisibleAnywhere)
		class UProjectHGameInstance* GI;


public:
	/* ------------------
		public Function
	--------------------*/
	void AddQuest(FNPCQuest NPCQuest); // ����Ʈ UI���� ������ ������ �߰�.

	void SelectQuest(FString SelectQuestName); // ����Ʈ�� ���� ����ų�, �ƿ� �����ʾ����� ����Ʈ�� ���õ�.
	// ����Ʈ�� �̹� �ѹ��̶� ���������� ����Ʈ ���ٲٰ�, �ƴϸ� �ٲܼ� �ִ�.

	void RemoveQuest(int32 RemoveQuestNumber); // �Ϸ��� ����Ʈ�� �����.
	// �� ����Ʈ �ý����� �ݵ�� �����ϴ� ����Ʈ�� �Ϸ�ǰ� �س����� �׷� ���� ������ٸ� Remove���� �ش� ����Ʈ �ε����� �޾ƿͼ� �����..

	void CompleteStep(int32 QuestNumber); // QuestStep�� ���� �ܰ�� �ѱ��.

	UFUNCTION()
		void AcceptQuest(); // FOnAcceptEvent ��������Ʈ�� �����Ǿ��ִ� �Լ�.


	void UpdateQuestSave(); // ����Ʈ�� �����Ǿ����� ���� ���̺� ���Ͽ� �����ϴ� �Լ�.

	void StartQuestLoad(); // �����Ҷ� ����Ʈ �ε��ϱ�.

	void BindSetDescription(int32 QuestIndex); // ���ͳ� Ƚ���� �ִ� ����Ʈ�� �Ҷ� ���� ����� ���ε�

	void SaveQuestNumber(FString NPCName, int32 QuestNumber);

	void BeginSetupHaveQuests(); // Ȥ���� ����Ʈ �ߺ� ������ �����ϱ� ���Ͽ� HaveQuestNumber�� ����.

	void NewTriggerSet(int32 QuestIndex); // ����Ʈ Ʈ���Ÿ� �Ϸ��ϸ� �״��� Ʈ���Ÿ� �����Ѵ�.

	/*-------------
		Get & Set
	--------------*/



	UFUNCTION(BlueprintCallable)
		FActiveQuest GetActiveQuest() { return ActiveQuest; }
	TArray<FQuestStruct> GetQuests() { return Quests; }
	int32 GetCurrentID() { return CurrentQuestID; }
	TSet<int32>* GetHaveQuestNums() { return &HaveQuestNumber; }

	void ActiveQuestClear() { ActiveQuest.Clear(); }

private:
	/* ----------------
		Private Value
	------------------*/

	UPROPERTY(VisibleAnywhere)
		FActiveQuest ActiveQuest; // ���� �������� ����Ʈ.

	UPROPERTY(VisibleAnywhere)
		TArray<FQuestStruct> Quests; // ��ü ������ �ִ� ����Ʈ ����Ʈ.

	UPROPERTY(VisibleAnywhere)
		TSet<int32> HaveQuestNumber; // ������ �ִ� ����Ʈ��ȣ�� �˻��ϱ� ���� ��.

	UPROPERTY(VisibleAnywhere)
		  int32 CurrentQuestID = -1; // ���� ����Ʈ�� ��ȣ


	/* ------------------
		Private Funtion
	--------------------*/

	void ResetQuest(); // ���� �����ϰ��ִ� ����Ʈ�� ����Ʈ ���̵� �ʱ�ȭ.
	void SelectTopQuest(int32 RemoveQuestNumber); // �����ϴ� ����Ʈ�� ���ٸ� �ڵ����� ���� ����� ����Ʈ ����.		
};
