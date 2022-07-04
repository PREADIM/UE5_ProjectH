// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "GameFramework/GameModeBase.h"
#include "Tema/JRPG/JRPGUnit.h"
#include "JRPGGameMode.generated.h"

/**
 * 
 */
USTRUCT()
struct FPriorityUnit
{
	GENERATED_USTRUCT_BODY()

public:
	FPriorityUnit();
	FPriorityUnit(class AJRPGUnit* Char);

	class AJRPGUnit* Unit;
	int32 Priority;
};


struct PriorityUnitFunc
{
	bool operator() (const FPriorityUnit& A, const FPriorityUnit& Other) const
	{
		return A.Priority > Other.Priority; // �ִ���
	}
};



UCLASS()
class PROJECTH_API AJRPGGameMode : public AGameModeBase
{
	GENERATED_BODY()


// ��Ʋ ���� -> ĳ���Ͱ� ������ �߰ų�, �޾����ÿ� JRPG���·� ���� ���� �Լ�.
// �� ����Ʈ �ʱ�ȭ -> ��� ���ֿ� ���� ���� �ʱ�ȭ. (��Ʋ ���۽ÿ� �� �ѹ� ���)
// �� �� ���� -> ����Ʈ�� ���� �����ִ� ������ �� ����.
// �� ������Ʈ ��Ʋ���� -> �� ������ ������ �ִ� ������Ʈ�� ��Ʋ ���� �Լ� ����.

// �� ���� -> ���� ����� �� ����� ������Ʈ���� ȣ���Ѵ�. ����Ʈ ������ �� �Ʒ��� �ִ´�.
// �� �ٽ� �� ���� �Լ��� ȣ���Ѵ�. 

// ���ܻ�Ȳ
// �� �ش� ������ ������ �׾��ִ� ��� -> �ش� ĳ���ͷ� ����Ʈ�� for range�� ã�Ƽ� ����. ���������� ����
// �� �ش� ������ ������ �����̻��� ��� -> �Ͻ��� �Լ����� ���ݰ����� �������� �Ǻ��Ѵ�.
// �� ���� �� �׾��ų�, �Ʊ��� �� �׾��� ��� -> ������ ���� ���� �Լ����� ���� ���׾����� �Ǻ�. �׸��� ������.
public:
	AJRPGGameMode();
	virtual void PostLogin(APlayerController* Login) override;



public:
	AJRPGUnit* GetCharacterSpawn(int32 CharacterNum, FTransform UnitLocation);
	AJRPGUnit* GetEnermySpawn(int32 CharacterNum, FTransform UnitLocation);
	bool GetBattleField(int32 FieldNum);
	void BattleStart(int32 FieldNum, TArray<int32> Enermys);
	void TurnStart();
	void TurnEnd();

	void TurnListInit();
	void TurnListSet();

	void GameEnd(); // �Ʊ��̳� ���� �� �׾��� ��� �ش� ������ ������.
	// ���̳� �Ʊ��̳� ��ó�� AJRPGUnit�̹Ƿ� TakeDamage���� �������� �ְ�, ĳ��Ʈ �ϸ� �ɵ��ϴ�.

	void ReturnWorld(); // ���ӳ����� ���� �������� ���ư���

	void SetOwnerUnits(); // �Ʊ� ���� ���� �� ����Ʈ �Ҵ�.
	void SetEnermyUnits(TArray<int32> Enermys); // ���� ���� ���� �� ����Ʈ �Ҵ�.

	void FirstStartSaveStat(); // ������ ����Ǿ��ִ� ���̺������� ���ٴ� ���� ���� ���� ĳ���Ͱ� ���ٴ� ���̹Ƿ�.
	// ���� ĳ���͸� ������.

	void SetControllerInit();
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class AJRPGPlayerController* OwnerController;
	UPROPERTY()
		TArray<FPriorityUnit> UnitList; 

	UPROPERTY()
		TArray<class AJRPGUnit*> EnermyUnits;
	UPROPERTY()
		TArray<class AJRPGUnit*> OwnerUnits;
	// ���̳� ĳ���Ͱ� ���ݹ޾� ��Ʋ start�� �ϱ����� ���� �� ���Ӹ�忡 �����ϰ� ���� �ְ� �Ѵ�.
	// �ᱹ�� �̰��� ĳ���;ȿ� �ִ� ���� �ִ°�. 
	// ���� ����ó�� ��ǥĳ���͸� �ٲܼ� �ִ� �����̶��, ĳ���͸� �����ϸ鼭 ���� ĳ���͵� ������.



	/* �׸����� ���� ���Ӹ�带 ���� �ν��Ͻ�ó�� ����ؾ���. ���̺� ���ϵ� ���� ���⿡ �ִ�. */
	UPROPERTY(VisibleAnywhere)
		class UDataTable* FieldTable; // �ʵ� ������ ���̺�
	UPROPERTY(VisibleAnywhere)
		class UDataTable* CharListTable; // JPRGĳ���� ���̺�.
	UPROPERTY(VisibleAnywhere)
		class UDataTable* EnermyListTable; // JPRGĳ���� ���̺�.


	UPROPERTY()
		class ABattleField* CurrentField;
	FVector FieldLocation;
	// ��Ʋ�ʵ� Ŭ������ ��������. ��Ʋ�ʵ� ���� ���� �ѹ��� �������ְ� �ش� ��ġ�� �����ͺ��̽����� ������.



	/*------------------
	
		Save & Load
	
	------------------*/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UJRPGSave* JRPGSave; // ���̺�

	void SetSaveJRPG();
	
};
