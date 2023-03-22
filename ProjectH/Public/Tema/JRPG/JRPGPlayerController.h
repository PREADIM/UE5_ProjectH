// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "PlayerControllerBase.h"
#include "Tema/JRPG/JRPGCharStat.h"
#include "Tema/JRPG/JRPGUnitUIStruct.h"
#include "Tema/JRPG/DebuffClass.h"
#include "JRPGPlayerController.generated.h"

/**
 * 
 */

struct FEnermy;

UCLASS()
class PROJECTH_API AJRPGPlayerController : public APlayerControllerBase
{
	GENERATED_BODY()

// ������ ���۵Ǹ� Ư�� ������ �̵��ؼ� �ش� �������� ������ �ϰ��Ѵ�. ��, ���ƴٴϴ� ���� �����͸� �������ʰ�, 
// �׳� ���� �ʿ��� ĳ���͸� �����ؼ� ���� ���̴�.


public:
	AJRPGPlayerController();

	virtual void BeginPlay();
	virtual void OnPossess(APawn* NewPawn) override;
	virtual void OnUnPossess() override;

	void SetupInputComponent();

	FJRPGUnitUIStruct* GetUnitUI(int32 CharacterNum);
	class UTexture2D* GetPartySettingIcon(int32 CharacterNum);


public:
	// ���� ������ �ΰ��̴�. ���� ������, JRPG���� ����. ���� �ٸ� �����̰� ��Ʋ�϶� �޶�����.
	void CameraPossess(FVector Location, FRotator Rotation, FTransform AllTargetTransform);

	UFUNCTION(BlueprintCallable)
		void CameraSetUp(FVector Location);
	UFUNCTION(BlueprintCallable)
		void CameraRotSetUp(FRotator Rotation);
	void ExitCamera();
	FVector GetCameraLocation();

	void GameEndSpawnCharacter();

	void OpenESC();

	UFUNCTION(BlueprintCallable)
		void MouseOnOff();

	/*UFUNCTION(BlueprintCallable)
		void MouseOn();
	UFUNCTION(BlueprintCallable)
		void MouseOff();*/


	void SetSave(); // ���Ӹ�忡 �����Ͽ� ���̺��ϱ�.
	void SetPartyChange(); // ��Ƽ ���������� �������� ��Ƽ ����.

	void SetParty_First();
	void SetParty_Second();
	void SetParty_Third();


	void SetRepreCharacterSpawn(int32 index); // �ٽ� ��ǥ ĳ���ͷ� OnPossess�ϴ� �Լ�
	void SetRepreCharacterSpawnUI(int32 index); // UI���� �ٽ�Character�� ���ƿ��� �Լ�.

	FJRPGCharStat GetCharStat(int32 CharNum);
	//���� ��忡 �ִ� GetCharStat�� ��Ʈ�ѷ����� �޾ƿ��� ���. �������� ���� ����.
	// �ᱹ�� Char�� ������ �������ִ� �� ��Ʈ�ѷ��̴� ���⼭ �޾ƿ��� �͵� ��������


	/*------------------------------------
			Drop Exp Or Character
	-------------------------------------*/

	void DropItem();
	void AddCharExp(int32 CharNum, float DropExp);
	void AddDropChar(int32 CharNum); 

	//--------------------------------------

	// ��Ʋ 
	UPROPERTY(VisibleAnywhere)
		bool bBattleING; // ��Ʋ ���� Ȯ��.
	bool GetBattleING() { return bBattleING; }

	bool PlayBattleMode(class AJRPGFieldEnermy* CurrentFieldEnermy); // ��Ʋ ����.
	void ReturnMainWidget(); // ��Ʋ ������ ���� �ʱ�ȭ.
	void WinGame();
	void RetrunToField(); // ��Ʋ ������ ó��

	void StartBattleWidget();
	void BattleTurnStart(bool bFlag);
	void SetVisibleBattleWidget(bool bFlag); // ��Ʋ���� ����ų� �ѱ�.
	void SkillAndListButtonHidden(bool bFlag); // �� �Ͽ��� �ʿ��� ������ Ű��

	void BattleESC(); // ��ų ��� ��, ������ ���� �ٿ��
	void VisibleDamage(float Damage, FVector TargetLocation); // ������ ����

	void UnitTurnEnd(); // �Ͽ��� ���ֿ��� �������Ʈ���� �ش� �Լ��� ȣ���Ѵ�.
	void EnermyListSetup(); // �ִ��� �� ������ �� �����Ŀ� ī�޶�� ������ �����ؾ��Ѵ�.
	void HiddenLockOn(); // ���� ����� ���� �뵵.

	void EnermySetupLockOnTargetUnit(class AJRPGUnit* Target); //���� ����Ű�� ���� ���ֿ� ������ ���� �Ѵ�.

public:
	UPROPERTY(VisibleAnywhere)
		class AJRPGGameMode* GM;
	UPROPERTY(VisibleAnywhere)
		class UAudioComponent* AudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AJRPGCamera> BP_Camera;
	UPROPERTY(VisibleAnywhere)
		class AJRPGCamera* DynamicCamera;

	UPROPERTY(VisibleAnywhere)
		FTransform FieldLocation;
	UPROPERTY(VisibleAnywhere)
		int32 RepreCharacterNum; // ��ǥĳ����
	UPROPERTY(VisibleAnywhere)
		class AJRPGUnit* RepreCharacter;
	UPROPERTY(VisibleAnywhere)
		class AJRPGFieldEnermy* CurrentOverlapFieldEnermy;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<int32> CurrentParty; // ���� ���õǾ��ִ� ��Ƽ����Ʈ
	UPROPERTY(VisibleAnywhere)
		int32 CurrentFieldNum; // �ʵ� ����
	UPROPERTY(VisibleAnywhere)
		int32 CurrentPartyIndex = 0; // ���� ���° �ε�������.
	FTransform AllTargetCameraTransform;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<int32> HaveCharList; // ������ �ִ� ��ü ĳ���� �ѹ� (���� ���̺� �ε� �ؾ���).
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)	
		TMap<int32, int32> HaveCharLevels; // ������ �ִ� ĳ������ ������. (������ ������ ���������̺��� �˻��ؼ� ������)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TMap<int32, FJRPGCharStat> CharStats; // ������ �ִ� ĳ������ ���ݵ�.
	UPROPERTY(VisibleAnywhere)
		TMap<int32, float> CurrentExp; // ���� ����ġ

	UPROPERTY(VisibleAnywhere)
		class UDataTable* UnitUITable;

	/*------------------------------
				Widget
	-------------------------------*/


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MainUI)
		TSubclassOf<class UJRPGTemaUI> BP_TemaMainUI;
	UPROPERTY()
		class UJRPGTemaUI* TemaMainUI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UDropItemWidget> BP_DropItemWidget;
	UPROPERTY()
		class UDropItemWidget* DropCharWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UDropExpWidget> BP_DropExpWidget;
	UPROPERTY()
		class UDropExpWidget* DropExpWidget;

	UPROPERTY()
		TArray<class UCustomWidget*> LastWidget; // ������ Widget�� ����ó�� �����ϴ� �迭.

	/*----------------------
		������ ������  Ÿ��
	-----------------------*/

	UPROPERTY(VisibleAnywhere)
		class AJRPGUnit* CurrentTurnUnit; /* ���� ������ Ÿ�� */
	UPROPERTY(BlueprintReadWrite)
		class AJRPGUnit* TargetUnit; // BattleWidget���� ���� ���� ������ TargetUnit;
	UPROPERTY(BlueprintReadWrite)
		TArray<class AJRPGUnit*> TargetUnits; /* ��ü ���ݿ� Ÿ�� */

	UFUNCTION(BlueprintCallable)
		void TargetToRotation(); /* ��� ����Ʈ���� TargetToRatation�� ����ؾ� �� ��� */
	UFUNCTION(BlueprintCallable)
		void AllTargetRoation(); /* ��ü ������ ���� �ʵ� AllShot ī�޶� ��ġ ���� */

	void OwnerUnitTurnToTarget(); /* �Ʊ� ������ Ÿ�ٿ��� ȸ���� ���ÿ� ���̳��� ī�޶� ȸ�� */
	void EnermyUnitTurnToTarget(); /* �� ������ Ÿ�ٿ��� ȸ���� ���ÿ� ���̳��� ī�޶� ȸ�� */

	/*-------------------------------------------*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UDamageWidget> BP_DamageSkin;

	/*--------------------------------
					������
	---------------------------------*/

	UPROPERTY()
		class ULevelSequencePlayer* SequencePlayer;
	UPROPERTY(EditAnywhere)
		class ULevelSequence* StartSequence;
	UPROPERTY(EditAnywhere)
		class ULevelSequence* EndSequence;

	float BattleStartSequence();
	// ��Ʋ �����Ҷ� ���� �κ����� �� ������ ����
	float BattleEndSequence();
	//��Ʋ ���� ���� ������.

	/*--------------------------------
			Ʃ�丮�� ����
	---------------------------------*/

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget> BP_BattleTutorialWidget;
	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget> BP_PartyTutorialWidget;

	void BattleTutorialStart();
	void PartyTutorialStart();

	/*------------------------------------
		��Ʋ ���� �ֹ߼� �ִϸ��̼� ����
	------------------------------------*/

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCustomAnimWidget> BP_BattleStartWidget;
	void CreateBattleStartWidget();

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCAWTextAnimWidget> BP_TurnEndCCStateWidget; // �� ���ᶧ CC���¸� ��Ÿ���� ���� �߰��ϱ�.
	void SetTurnEndDebuffWidget(ECCType CCState);

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCustomAnimWidget> BP_BattleESCWidget;
	UPROPERTY()
		class UCustomAnimWidget* BattleESCWidget;
	bool bBattleBeginning = false; /* ��Ʋ �������� ����Ǵ� ������ ���ӿ��� ������ ���� �Ѵ�.*/
	
	//-------------------------------------------

	void BattleUIOnOff(bool bOnOff); // UI �ѱ� ����.
	void PlayPriority();

	void SetupDropCharWidget(int32 DropCharNum);
	void SetupDropExpWidget(int32 DropExp);


	/*------------------------------------
				�����
	------------------------------------*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundBase* NormalSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundBase* BattleStartSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundBase* BattleSound;

	UFUNCTION()
		void SetPlaySound(USoundBase* Sound);
};

