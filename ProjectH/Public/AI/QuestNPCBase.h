// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "GameFramework/Pawn.h"
#include "QuestDataBase.h"
#include "Dialogue/TextNAnim.h"
#include "ActorComponent/QuestComponent/NPCInterface.h"
#include "QuestNPCBase.generated.h"


USTRUCT(BlueprintType)
struct FNormalDialogue
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere)
		TArray<FTextNName> Dialogue;
};

USTRUCT()
struct FNPCQuestPTR
{
	GENERATED_USTRUCT_BODY()
public:
	FNPCQuest* NPCQuestPTR;
};


// ����Ʈ�� ���¿����� ������ ��� ����� ��Ÿ�� ���� ������
UENUM()
enum class EQuestIconState : uint8
{
	NONE UMETA(DisplayName = "NONE"),

	SubCanQuest UMETA(DisplayName = "SubCanQuest"),
	MainCanQuest UMETA(DisplayName = "MainCanQuest"),

	SubQuesting UMETA(DisplayName = "SubQuesting"),
	MainQuesting UMETA(DisplayName = "MainQuesting"),

	SubSucceedQuest UMETA(DisplayName = "SubSucceedQuest"),
	MainSucceedQuest UMETA(DisplayName = "MainSucceedQuest")
};


UCLASS()
class PROJECTH_API AQuestNPCBase : public APawn, public INPCInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AQuestNPCBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay();
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


public:
	/*-----------------
		Public Value
	-----------------*/
	/*
	* ����Ʈ�� �⺻������ �������� -> ������ -> �Ϸᰡ�� -> �Ϸ� �����̴�.
	* 
	NPCQuests ��ü�� �Ϸ�� �������̴� �ϴ� NPC�� ��� ����Ʈ�� �������ִ´�.
	QuestingNums�� SucceedQuestsNums�� ���� ����Ʈ'��'�� �ѹ��� �Ϸᰡ '����'�� ����Ʈ���� �ѹ��̴�.
	�ش� ��ȣ���� NPCQuestPtrs�� �ѹ��� �˻��ؼ� �ش� �÷��׵��� ���ش�.

	����Ʈ�� �Ϸ� ���������� SucceedQuestsNums�� ��ȣ�� �߰��ϰ� QuestingNums���� ��ȣ�� �����.
	�׸��� ����Ʈ �Ϸ� �Ľ� EndedQuestsNums�� ��ȣ�� �߰��ϰ� SucceedQuestsNums���� ��ȣ�� �����.
	��(QuestingNums -> SucceedQuestsNums -> EndedQuestsNums�� �߰��Ǹ鼭 �������� ����)
	*/


	/*-----------------------
			Component
	------------------------*/

	UPROPERTY(EditAnywhere, Category = Component)
		class USceneComponent* Root;

	UPROPERTY(EditAnywhere, Category = Component)
		class USceneComponent* IconLocationComponent;

	/*------------------------------------------*/

	UPROPERTY(VisibleAnywhere)
		EQuestIconState QuestIconState;
	/**/
	int32 RetIconPriority(EQuestIconState State); // �� ������ ���¿� ����ġ�� ������ ����ؼ� �켱���� �Ǵ��Ѵ�.
	void SetQuestIconState(EQuestIconState NewState); // QuestIconState�� ����ġ�� �Ǵ��ؼ� ����Ͽ� �� ����.
	/**/


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FNPCAllQuest NPCQuests; // ������ ���� ����Ʈ ��ϵ�.

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TSet<int32> QuestingNums; // ����Ʈ ���� �ѹ��� (���̺� �ε�)

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TSet<int32> SucceedQuestsNums; // �Ϸᰡ�� ����Ʈ �ѹ���. (���̺� �ε�)
	// �Ϸᰡ��'��' ����Ʈ�� �ƴ϶� �Ϸ� ����Ʈ�̴�.

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TSet<int32> EndedQuestsNums; // �Ϸ�'��' ����Ʈ �ѹ���. (���̺� �ε�)

	UPROPERTY(VisibleAnywhere)
		TMap<int32, FNPCQuestPTR> NPCQuestPtrs; // ����Ʈ �ѹ��� �ش� ����Ʈ�� ���� �Ҽ� �ִ� �ּ� ������.
	// �ش� ���� ���� �Ϸ��� ����Ʈ�� �����ؼ� �Ϸ�Ʈ���Ÿ� �������ش�.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Name)
		FString NPCName; // NPC �̸�. ������ ���̽� �˻��̳� �����Ҷ� �����ϴ�.

	/*-----------------------
		Quest Icon Widget
	------------------------*/

	UPROPERTY(EditAnywhere, Category = Widget)
		TSubclassOf<class UNormalIconUI> BP_NPCQuestIconUI;
	UPROPERTY()
		class UNormalIconUI* NPCQuestIconUI;
	UPROPERTY()
		class UCanvasPanelSlot* NPCIconSlot;

	/*---------------------------------------------------*/

	UPROPERTY(VisibleAnywhere)
		class UProjectHGameInstance* GI;


	/*--------------------
		Quest Property
	--------------------*/

	//
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class AProjectH_PC* PlayerController;

	/*--------------------
		Dialogue Property
	--------------------*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FNormalDialogue NormalDialogue;

public:
	/*-------------------
		Public Function
	--------------------*/

	virtual void Interact_Implementation(class AProjectHCharacter* OwnerCharacter);

	void QuestInfoOpen(int32 QuestIndex, class AProjectH_PC* OwnerController);
	
	void QuestSucceedInfoOpen(int32 QuestIndex, class AProjectH_PC* OwnerController);

	void NPCQuestSetup(); // �ѹ��� �ݵ�� ����Ǿ�� �� �Ʒ� �߿� �Լ����� ��Ƶа�.
	//FindCanQuest, SetIconWidget, SaveNPCQuest

	/* �ٽ� �Լ�. */ 
	bool FindCanQuest(); // ĳ���Ͱ� �ش� NPC�� ����Ʈ�� ���������� �������� �˻��Ѵ�.
	void SaveNPCQuest(); // ���� �ν��Ͻ� �ȿ��ִ� ���̺� Ŭ������ �����ϴ� �Լ�.
	void SetIconWidget(); // ������ ���.
	void HiddenIcon(); // ������ ������.
};
