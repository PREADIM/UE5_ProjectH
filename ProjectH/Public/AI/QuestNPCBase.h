// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "GameFramework/Pawn.h"
#include "NPCAllQuest.h"
#include "Dialogue/TextNAnim.h"
#include "ActorComponent/QuestComponent/NPCInterface.h"
#include "QuestNPCBase.generated.h"


USTRUCT(BlueprintType)
struct FNormalDialogue
{
	GENERATED_USTRUCT_BODY()
public:
	/*UPROPERTY(EditAnywhere)
		TArray<FText> Dialogue;*/

	UPROPERTY(EditAnywhere)
		TArray<FTextNName> Dialogue;

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
	//virtual void Tick(float DeltaTime) override;


public:
	/*-------------------
		public Function
	---------------------*/

	void SetNPCQuests(FNPCAllQuest& LoadQuests) { NPCQuests = LoadQuests; }


public:
	/*-----------------
		Public Value
	-----------------*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FNPCAllQuest NPCQuests; // ������ ���� ����Ʈ ��ϵ�.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component)
		class USceneComponent* Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component)
		class UCapsuleComponent* CapsuleCollision;

	/*-----------------------
		Quest Widget Value
	------------------------*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component)
		class UWidgetComponent* IconWidget; // �׳� ����Ʈ ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component)
		class UWidgetComponent* MainIconWidget; // ���� ����Ʈ ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component)
		class UWidgetComponent* SucceedWidget; // �Ϸ� ������

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Name)
		FString NPCName; // NPC �̸�. ������ ���̽� �˻��̳� �����Ҷ� �����ϴ�.
	UPROPERTY(VisibleAnywhere)
		class UProjectHGameInstance* GI;


	/*--------------------
		Quest Property
	--------------------*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bQuestSucceed; // ����Ʈ �Ϸ��ΰ�?
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bIsQuesting; // ����Ʈ ���ΰ�?
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bCanAccept; // ĳ���Ͱ� ����Ʈ�� ���������� NPC�ΰ�? (���� �����)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int32 CanQuestCnt; // �󸶳� ���������� ����Ʈ�� ������?
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int32 CanMainQuestCnt; // �󸶳� ���������� ����Ʈ�� ������?

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bHaveMainQuest; // ���� ����Ʈ�� �������ִ°�? 

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class AProjectHCharacter* PlayerCharacter;


	/*--------------------
		Dialogue Property
	--------------------*/

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FNormalDialogue> NormalDialogue;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FNormalDialogue NormalDialogue;


public:
	/*-------------------
		Public Function
	--------------------*/

	void AddNPCQuest(FNPCQuest Quest, bool bSucceed); // NPC ����Ʈ ���� �ǽð����� �����ϱ� �Լ�.

	virtual void Interact_Implementation(class AProjectHCharacter* OwnerCharacter);

	void QuestInfoOpen(int32 QuestIndex, class AProjectH_PC* OwnerController);
	void SaveNPCQuest(); // ���� �ν��Ͻ� �ȿ��ִ� ���̺� Ŭ������ �����ϴ� �Լ�.

	void SetIconWidget();
	void HiddenIcon();

	bool FindCanQuest(); // ĳ���Ͱ� �ش� NPC�� ����Ʈ�� ���������� �������� �˻��Ѵ�.

	bool CanVisibleWidget(); 
	// ������ ����� �Ǵ��� �Ǵ��Ѵ�. ĳ����Ŭ�������� NPC�� ������ ������Ҷ� �Ǵ��ϴ� �Լ�.

};
