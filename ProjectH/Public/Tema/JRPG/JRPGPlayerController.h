// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "GameFramework/PlayerController.h"
#include "Tema/JRPG/JRPGCharStat.h"
#include "Tema/JRPG/JRPGUnitUIStruct.h"
#include "JRPGPlayerController.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EGameModeType : uint8
{

	Normal UMETA(DisplayName = "Normal"),
	Battle UMETA(DisplayName = "Battle"),
	UI UMETA(DisplayName = "UI")
};



UCLASS()
class PROJECTH_API AJRPGPlayerController : public APlayerController
{
	GENERATED_BODY()

// ������ ���۵Ǹ� Ư�� ������ �̵��ؼ� �ش� �������� ������ �ϰ��Ѵ�. ��, ���ƴٴϴ� ���� �����͸� �������ʰ�, 
// �׳� ���� �ʿ��� ĳ���͸� �����ؼ� ���� ���̴�.


public:
	AJRPGPlayerController();

	virtual void BeginPlay();
	virtual void OnPossess(APawn* NewPawn) override;

	void SetupInputComponent();

	FJRPGUnitUIStruct* GetUnitUI(int32 CharacterNum);
	class UTexture2D* GetPartySettingIcon(int32 CharacterNum);


public:
	// ���� ������ �ΰ��̴�. ���� ������, JRPG���� ����. ���� �ٸ� �����̰� ��Ʋ�϶� �޶�����.

	void CameraPossess(FVector Location, FRotator Rotation);
	void CameraSetUp(FVector Location);
	void CameraRotSetUp(FRotator Rotation);
	void ExitCamera();
	FVector GetCameraLocation();

	void GameEndSpawnCharacter();

	void OpenESC();
	void MouseOnOff();
	void MouseOn();
	void MouseOff(); // �̷��� ���� �δ� ������ �ʿ信���� ����ؼ� ���� On Off�� �ϱ� ���ؼ�.


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

	

	// ��Ʋ 
	void PlayBattleMode(TArray<int32> EnermyUnits); // ��Ʋ ����.
	void ReturnMainWidget(); // ��Ʋ ������ ���ư���.

	void StartBattleWidget();
	void BattleTurnStart(bool bFlag);
	void SetVisibleBattleWidget(bool bFlag); // ��Ʋ���� ����ų� �ѱ�.
	void SetEnermyTurnWidget(bool bFlag); // �� �Ͽ��� �ʿ��� ������ Ű��

	void BattleESC(); // ��ų ��� ��, ������ ���� �ٿ��
	void VisibleDamage(float Damage, FVector TargetLocation); // ������ ����


	void UnitTurnEnd(); // �Ͽ��� ���ֿ��� �������Ʈ���� �ش� �Լ��� ȣ���Ѵ�.

	void EnermyListSetup(); // �ִ��� �� ������ �� �����Ŀ� ī�޶�� ������ �����ؾ��Ѵ�.

public:
	UPROPERTY(VisibleAnywhere)
		class AJRPGGameMode* GM;

	UPROPERTY(VisibleAnywhere)
		EGameModeType GameType;

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


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<int32> CurrentParty; // ���� ���õǾ��ִ� ��Ƽ����Ʈ
	UPROPERTY(VisibleAnywhere)
		int32 CurrentFieldNum; // �ʵ� ����
	UPROPERTY(VisibleAnywhere)
		int32 CurrentPartyIndex = 0; // ���� ���° �ε�������.

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<int32> HaveCharList; // ������ �ִ� ��ü ĳ���� �ѹ� (���� ���̺� �ε� �ؾ���).
	// ���߿� ����Ʈ �ѹ�ó�� ��ȣ�� BP �н��� ������ �ش� �ѹ��� �����ϰ� �ҷ����� �������� �����ɵ�.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)	
		TMap<int32, int32> HaveCharStat; // ������ �ִ� ĳ������ ������. (������ ������ ���������̺��� �˻��ؼ� ������)
	//TMap<int32, FJRPGCharStat> HaveCharStat; // ������ �ִ� ĳ������ ���ݵ�.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MainUI)
		TSubclassOf<class UJRPGTemaUI> BP_TemaMainUI;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MainUI)
		class UJRPGTemaUI* TemaMainUI;

	UPROPERTY(VisibleAnywhere)
		class UDataTable* UnitUITable;


	UPROPERTY()
		TArray<class UCustomWidget*> LastWidget; // ������ Widget�� ����ó�� �����ϴ� �迭.
	// AddToViewport �� ����ϴ� UI�� �����Ѵ�. Animation�� �������ִ� �̹� �Ҵ�Ǿ��ִ� UI�� ����.


	UPROPERTY(VisibleAnywhere)
		class AJRPGUnit* CurrentUnit; // ESC�� �� Unit ��ų�� ���� ���̸� ����ϰ� , �ƹ��͵� ���������� ������ ������ â�� ����.

	UPROPERTY(BlueprintReadWrite)
		class AJRPGUnit* TargetUnit; // BattleWidget���� ���� ���� ������ TargetUnit;
	UPROPERTY(BlueprintReadWrite)
		TArray<class AJRPGUnit*> TargetUnits;
	UFUNCTION(BlueprintCallable)
		void TargetToRotation();
	UFUNCTION(BlueprintCallable)
		void EnermyTargetToRotation();


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UDamageWidget> BP_DamageSkin;
	

};
