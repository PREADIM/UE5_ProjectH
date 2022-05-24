// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Components/ActorComponent.h"
#include "QuestStruct.h"
#include "QuestStep.h"
#include "NPCQuest.h"
#include "QuestComponent.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnAddedRemovedQuest); // ����Ʈ�� Ŭ�����ϰų� ���� �߰��Ҷ�
DECLARE_MULTICAST_DELEGATE(FOnActiveQuestChange); // ������ ����Ʈ�� ����������
DECLARE_MULTICAST_DELEGATE(FOnAcceptEvent); 
DECLARE_MULTICAST_DELEGATE(FOnCompleteStep); // ����Ʈ �Ϸ� ������
DECLARE_MULTICAST_DELEGATE(FOnSetDescription); // Ƚ���� �ִ� ����Ʈ�� �Ҷ� ����� ��ȭ.


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
	
	FOnAddedRemovedQuest OnAddedRemovedQuest; // ����Ʈ�� �߰� �� ���� �Ǿ����� ������ ��������Ʈ.
	FOnActiveQuestChange ActiveQuestChange; // ��Ƽ�� ����Ʈ�� ����Ǿ����� ������ ��������Ʈ
	FOnAcceptEvent AcceptEvent; // ����Ʈ ������ �ÿ� ȣ��� �Լ���
	FOnCompleteStep CompleteQuestDelegate; // ����Ʈ �� �ܰ� Ŭ����� ȣ���� ��������Ʈ.
	FOnSetDescription SetDescriptionDelegate; // Ƚ���� �ִ� ����Ʈ�� �Ҷ� ����� ��ȭ.

	/* ������ ����Ʈ�� NPC�� �����ͼ� NPC�� Index°�� ����Ʈ�� �����.
		�Ǵ� ����Ʈ Ʈ���ſ� �ش� NPC�� ������ �Ѱ��ش�.*/
	UPROPERTY()
		class AQuestNPCBase* NewNPC;
	int32 NewNPCQuestIndex;


public:
	/* ------------------
		public Function
	--------------------*/
	void AddQuest(FNPCQuest NPCQuest); // ����Ʈ UI���� ������ ������ �߰�.

	void SelectQuest(FString SelectQuestName); // ����Ʈ�� ���� ����ų�, �ƿ� �����ʾ����� ����Ʈ�� ���õ�.
	// ����Ʈ�� �̹� �ѹ��̶� ���������� ����Ʈ ���ٲٰ�, �ƴϸ� �ٲܼ� �ִ�.

	void RemoveQuest(); // �Ϸ��� ����Ʈ�� �����.

	void CompleteStep(); // QuestStep�� ���� �ܰ�� �ѱ��.

	UFUNCTION()
		void UpdateQuestCashes(); // ����Ʈ ���� ã�� �� �ֵ��� ĳ�� ����.
	UFUNCTION()
		void AcceptQuest(); // FOnAcceptEvent ��������Ʈ�� �����Ǿ��ִ� �Լ�.


	void UpdateQuestSave(); // ����Ʈ�� �����Ǿ����� ���� ���̺� ���Ͽ� �����ϴ� �Լ�.

	void StartQuestLoad(); // �����Ҷ� ����Ʈ �ε��ϱ�.

	void BindSetDescription(); // ���ͳ� Ƚ���� �ִ� ����Ʈ�� �Ҷ� ���� ����� ���ε�

	void FinishedQuestNumber(int32 QuestNumber); // ����Ʈ �Ϸ��ϸ� ������ �Լ�.
	void SaveQuestNumber(int32 QuestNumber);
	// NPC �̸����� ������ �ν��Ͻ� �ȿ��ִ� TMap���� ���� �ص��ɵ� �ϴ�. �ڡ�
	// ���� �̰��ӿ����� �ʿ������ ������ ���� �������� ����.




	/*-------------
		Get & Set
	--------------*/

	FQuestStruct GetActiveQuest() { return ActiveQuest; }
	TArray<FQuestStruct> GetQuests() { return Quests; }
	TMap<FString, FQuestStruct> GetQuestCashes() { return QuestCashes; }
	int32 GetCurrentID() { return CurrentQuestID; }





private:
	/* ----------------
		Private Value
	------------------*/
	UPROPERTY(VisibleAnywhere)
		FQuestStruct ActiveQuest; // ���� �������� ����Ʈ.

	UPROPERTY(VisibleAnywhere)
		TArray<FQuestStruct> Quests; // ��ü ������ �ִ� ����Ʈ ����Ʈ.

	UPROPERTY(VisibleAnywhere)
		TMap<FString, FQuestStruct> QuestCashes;

	UPROPERTY(VisibleAnywhere)
		  int32 CurrentQuestID = -1; // ���� ����Ʈ�� ��ȣ

	UPROPERTY(VisibleAnywhere)
		class AProjectHCharacter* OwnerCharacter;

	UPROPERTY(VisibleAnywhere)
		class UProjectHGameInstance* GI;

	/* ------------------
		Private Funtion
	--------------------*/

	void ResetQuest(); // ���� �����ϰ��ִ� ����Ʈ�� ����Ʈ ���̵� �ʱ�ȭ.
	void SelectTopQuest(); // �����ϴ� ����Ʈ�� ���ٸ� �ڵ����� ���� ����� ����Ʈ ����.


		
};
