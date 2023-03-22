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

// 전투가 시작되면 특정 맵으로 이동해서 해당 구역에서 전투를 하게한다. 즉, 돌아다니던 원본 데이터를 지우지않고, 
// 그냥 전투 맵에서 캐릭터를 생성해서 쓰는 것이다.


public:
	AJRPGPlayerController();

	virtual void BeginPlay();
	virtual void OnPossess(APawn* NewPawn) override;
	virtual void OnUnPossess() override;

	void SetupInputComponent();

	FJRPGUnitUIStruct* GetUnitUI(int32 CharacterNum);
	class UTexture2D* GetPartySettingIcon(int32 CharacterNum);


public:
	// 메인 위젯이 두개이다. 평상시 위젯과, JRPG때의 위젯. 서로 다른 위젯이고 배틀일때 달라진다.
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


	void SetSave(); // 게임모드에 접근하여 세이브하기.
	void SetPartyChange(); // 파티 변경했으니 위젯에서 파티 변경.

	void SetParty_First();
	void SetParty_Second();
	void SetParty_Third();


	void SetRepreCharacterSpawn(int32 index); // 다시 대표 캐릭터로 OnPossess하는 함수
	void SetRepreCharacterSpawnUI(int32 index); // UI에서 다시Character로 돌아오는 함수.

	FJRPGCharStat GetCharStat(int32 CharNum);
	//게임 모드에 있는 GetCharStat을 컨트롤러에서 받아오는 방법. 위젯에서 쓸모 있음.
	// 결국엔 Char의 레벨을 가지고있는 이 컨트롤러이니 여기서 받아오는 것도 괜찮을듯


	/*------------------------------------
			Drop Exp Or Character
	-------------------------------------*/

	void DropItem();
	void AddCharExp(int32 CharNum, float DropExp);
	void AddDropChar(int32 CharNum); 

	//--------------------------------------

	// 배틀 
	UPROPERTY(VisibleAnywhere)
		bool bBattleING; // 배틀 상태 확인.
	bool GetBattleING() { return bBattleING; }

	bool PlayBattleMode(class AJRPGFieldEnermy* CurrentFieldEnermy); // 배틀 시작.
	void ReturnMainWidget(); // 배틀 종료후 위젯 초기화.
	void WinGame();
	void RetrunToField(); // 배틀 종료후 처리

	void StartBattleWidget();
	void BattleTurnStart(bool bFlag);
	void SetVisibleBattleWidget(bool bFlag); // 배틀위젯 숨기거나 켜기.
	void SkillAndListButtonHidden(bool bFlag); // 적 턴에서 필요한 위젯만 키기

	void BattleESC(); // 스킬 취소 및, 나가기 위젯 뛰우기
	void VisibleDamage(float Damage, FVector TargetLocation); // 데미지 띄우기

	void UnitTurnEnd(); // 턴엔드 유닛에서 블루프린트에서 해당 함수를 호출한다.
	void EnermyListSetup(); // 애님이 다 끝나고 턴 시작후에 카메라랑 락온을 변경해야한다.
	void HiddenLockOn(); // 게임 종료시 제거 용도.

	void EnermySetupLockOnTargetUnit(class AJRPGUnit* Target); //적이 가리키는 나의 유닛에 락온이 가게 한다.

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
		int32 RepreCharacterNum; // 대표캐릭터
	UPROPERTY(VisibleAnywhere)
		class AJRPGUnit* RepreCharacter;
	UPROPERTY(VisibleAnywhere)
		class AJRPGFieldEnermy* CurrentOverlapFieldEnermy;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<int32> CurrentParty; // 현재 선택되어있는 파티리스트
	UPROPERTY(VisibleAnywhere)
		int32 CurrentFieldNum; // 필드 정보
	UPROPERTY(VisibleAnywhere)
		int32 CurrentPartyIndex = 0; // 현재 몇번째 인덱스인지.
	FTransform AllTargetCameraTransform;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<int32> HaveCharList; // 가지고 있는 전체 캐릭터 넘버 (추후 세이브 로드 해야함).
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)	
		TMap<int32, int32> HaveCharLevels; // 가지고 있는 캐릭터의 레벨들. (레벨로 스탯을 데이터테이블에서 검색해서 가져옴)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TMap<int32, FJRPGCharStat> CharStats; // 가지고 있는 캐릭터의 스텟들.
	UPROPERTY(VisibleAnywhere)
		TMap<int32, float> CurrentExp; // 현재 경험치

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
		TArray<class UCustomWidget*> LastWidget; // 마지막 Widget을 스택처럼 저장하는 배열.

	/*----------------------
		전투시 공격할  타겟
	-----------------------*/

	UPROPERTY(VisibleAnywhere)
		class AJRPGUnit* CurrentTurnUnit; /* 현재 턴중인 타겟 */
	UPROPERTY(BlueprintReadWrite)
		class AJRPGUnit* TargetUnit; // BattleWidget에서 정한 현재 공격할 TargetUnit;
	UPROPERTY(BlueprintReadWrite)
		TArray<class AJRPGUnit*> TargetUnits; /* 전체 공격용 타켓 */

	UFUNCTION(BlueprintCallable)
		void TargetToRotation(); /* 블루 프린트에서 TargetToRatation을 사용해야 할 경우 */
	UFUNCTION(BlueprintCallable)
		void AllTargetRoation(); /* 전체 공격을 위한 필드 AllShot 카메라에 위치 조정 */

	void OwnerUnitTurnToTarget(); /* 아군 유닛을 타겟에게 회전과 동시에 다이나믹 카메라 회전 */
	void EnermyUnitTurnToTarget(); /* 적 유닛을 타겟에게 회전과 동시에 다이나믹 카메라 회전 */

	/*-------------------------------------------*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UDamageWidget> BP_DamageSkin;

	/*--------------------------------
					시퀀스
	---------------------------------*/

	UPROPERTY()
		class ULevelSequencePlayer* SequencePlayer;
	UPROPERTY(EditAnywhere)
		class ULevelSequence* StartSequence;
	UPROPERTY(EditAnywhere)
		class ULevelSequence* EndSequence;

	float BattleStartSequence();
	// 배틀 시작할때 위젯 인비지블 및 시퀀서 실행
	float BattleEndSequence();
	//배틀 종료 후의 시퀀스.

	/*--------------------------------
			튜토리얼 위젯
	---------------------------------*/

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget> BP_BattleTutorialWidget;
	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget> BP_PartyTutorialWidget;

	void BattleTutorialStart();
	void PartyTutorialStart();

	/*------------------------------------
		배틀 관련 휘발성 애니메이션 위젯
	------------------------------------*/

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCustomAnimWidget> BP_BattleStartWidget;
	void CreateBattleStartWidget();

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCAWTextAnimWidget> BP_TurnEndCCStateWidget; // 턴 종료때 CC상태를 나타내는 위젯 뜨게하기.
	void SetTurnEndDebuffWidget(ECCType CCState);

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCustomAnimWidget> BP_BattleESCWidget;
	UPROPERTY()
		class UCustomAnimWidget* BattleESCWidget;
	bool bBattleBeginning = false; /* 배틀 시퀀스가 실행되는 동안은 게임에서 나갈수 없게 한다.*/
	
	//-------------------------------------------

	void BattleUIOnOff(bool bOnOff); // UI 켜기 끄기.
	void PlayPriority();

	void SetupDropCharWidget(int32 DropCharNum);
	void SetupDropExpWidget(int32 DropExp);


	/*------------------------------------
				오디오
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

