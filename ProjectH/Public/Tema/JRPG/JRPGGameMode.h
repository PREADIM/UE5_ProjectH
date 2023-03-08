// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "GameFramework/GameModeBase.h"
#include "Tema/JRPG/JRPGCharStat.h"
#include "Tema/JRPG/JRPGDropStruct.h"
#include "JRPGGameMode.generated.h"

/**
 * 
 */

struct FEnermys;

USTRUCT(BlueprintType)
struct FPriorityUnit
{
	GENERATED_USTRUCT_BODY()

public:
	FPriorityUnit();
	FPriorityUnit(class AJRPGUnit* Char);

	UPROPERTY(BlueprintReadWrite)
		class AJRPGUnit* Unit;
	UPROPERTY(BlueprintReadWrite)
		int32 Priority;
};


struct PriorityUnitFunc
{
	bool operator()(const FPriorityUnit& A, const FPriorityUnit& Other) const
	{
		return A.Priority > Other.Priority; // �ִ���
	}
};


USTRUCT(BlueprintType)
struct FBuffIconStruct : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString IconName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UTexture2D* IconTexture;
};



UCLASS()
class PROJECTH_API AJRPGGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AJRPGGameMode();
	virtual void PostLogin(APlayerController* Login) override;

public:


	class AJRPGUnit* GetCharacterSpawn(int32 CharacterNum, FTransform UnitLocation);
	class AJRPGUnit* GetEnermySpawn(int32 CharacterNum, FTransform UnitLocation);
	bool GetBattleField(int32 FieldNum);
	FBuffIconStruct* GetBuffIcon(FString IconName);


	/*------------------------------
		JRPG ���� �帧 �Լ���
	-------------------------------*/

	void BattleStart(int32 FieldNum, TArray<FEnermys> Enermys);
	void TurnStart();

	UFUNCTION(BlueprintCallable)
		void TurnEnd();

	void TurnListInit();
	void SetUnitListArray(); // �� ���ķ� �켱���� ���� �� ���� ť�� ����.
	void TurnListSet();
	void GameEnd(bool bWinner); // �Ʊ��̳� ���� �� �׾��� ��� �ش� ������ ������.
	// true�� �÷��̾ ����, false�� ���� �¸�.


	void ReturnWorld(); // ���ӳ����� ���� �������� ���ư���

	void SetOwnerUnits(); // �Ʊ� ���� ���� �� ����Ʈ �Ҵ�.
	void SetEnermyUnits(TArray<FEnermys> Enermys); // ���� ���� ���� �� ����Ʈ �Ҵ�.

	void SetControllerInit();


public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class AJRPGPlayerController* OwnerController;
	UPROPERTY()
		TArray<FPriorityUnit> UnitList; // �켱���� �������� ���� �迭
	UPROPERTY()
		TArray <FPriorityUnit> SetUnitList; // ���ĵ� ���� ���⿡ �������� �����ؼ� ���

	UPROPERTY(BlueprintReadWrite)
		TArray<FPriorityUnit> EnermyUnits;
	UPROPERTY(BlueprintReadWrite)
		TArray<class AJRPGUnit*> EnermyList;
	// ���� ����ִ� ��. UI�ε� ����

	UPROPERTY(BlueprintReadWrite)
		TArray<FPriorityUnit> OwnerUnits;
	UPROPERTY(BlueprintReadWrite)
		TArray<class AJRPGUnit*> OwnerList; // �Ʊ��� �������� ����ִ� ��.
	


	//----------------------------------------------------------

	FJRPGCharStat GetCharStat(int32 CharNum, int32 Level); //ĳ���� ������ ������ �˻��ؼ� �����´�.

	void SetControllerInCharacterStat(); // �������ִ� ������ ������ �����´�.

	//----------------------------------------------------------


	/* �׸����� ���� ���Ӹ�带 ���� �ν��Ͻ�ó�� ����ؾ���. ���̺� ���ϵ� ���� ���⿡ �ִ�. */
	UPROPERTY(VisibleAnywhere)
		class UDataTable* FieldTable; // �ʵ� ������ ���̺�
	UPROPERTY(VisibleAnywhere)
		class UDataTable* CharListTable; // JPRGĳ���� ���̺�.
	UPROPERTY(VisibleAnywhere)
		class UDataTable* EnermyListTable; // JPRGĳ���� ���̺�.
	UPROPERTY(VisibleAnywhere)
		class UDataTable* CharStatTablePaths; // ĳ���� ������ �ִ� ���̺���� ��ε�.
	UPROPERTY(VisibleAnywhere)
		class UDataTable* FieldEnermyDropTable; // �ʵ� ���� ������ ����ġ �� ������.
	UPROPERTY(VisibleAnywhere)
		class UDataTable* IconTextureTable; // ���� ���� ������ ���̺�



	UPROPERTY(BlueprintReadOnly)
		class ABattleField* CurrentField;
	FVector FieldLocation;
	// ��Ʋ�ʵ� Ŭ������ ��������. ��Ʋ�ʵ� ���� ���� �ѹ��� �������ְ� �ش� ��ġ�� �����ͺ��̽����� ������.


	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget> BP_BattleTutorialWidget;
	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget> BP_PartyTutorialWidget;


	void BattleTutorialStart();
	void PartyTutorialStart();

	/*------------------
	
		Save & Load
	
	------------------*/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UJRPGSave* JRPGSave; // ���̺�

	bool bBattleTutorial = false; // Ʃ�丮���� �ߴ��� ���ߴ��� �˾ƿ��� ��.
	bool bPartyTutorial = false; // Ʃ�丮���� �ߴ��� ���ߴ��� �˾ƿ��� ��.

	UPROPERTY(EditAnywhere)
		int32 MaxCharCnt; // �� ������ ������ �ִ� ĳ���� ��
	// ������ Ȱ�� x ��߿� � ���� Ȯ���� �� �ʿ��� ��.

	void SetSaveJRPG();

	void SetSaveEnermyUnits(class AJRPGEnermy* FieldEnermy);
	bool GetSaveEnermyUnits(int32 EnermyUnitNum); 
	// �� ���� �ʵ��� ���� �׾����� ��Ҵ��� ���̺��ϴ� �Լ���.

	FJRPGDropStruct GetFieldEnermyDropStruct(int32 EnermyUnitNum);
	// �ʵ� ���� ���� �� ���� ������.

	UPROPERTY()
		class AJRPGEnermy* CurrentBattleEnermy; // ���� �ο�� �ʵ� ������ ����.
	// ������ ������ �׾����� ��Ҵ��� �˾ƾ� �ϱ⶧��.
	
	UPROPERTY(BlueprintReadOnly)
		int32 KillCnt; // ���� ����� ����. ī��Ʈ
};
