// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "PlayerControllerBase.h"
#include "Tema/JRPG/JRPGCharStat.h"
#include "Tema/JRPG/JRPGUnitUIStruct.h"
#include "JRPGPlayerController.generated.h"

/**
 * 
 */

struct FEnermy;

UENUM(BlueprintType)
enum class EGameModeType : uint8
{

	Normal UMETA(DisplayName = "Normal"),
	Battle UMETA(DisplayName = "Battle"),
	UI UMETA(DisplayName = "UI")
};



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

	void SetupInputComponent();

	FJRPGUnitUIStruct* GetUnitUI(int32 CharacterNum);
	class UTexture2D* GetPartySettingIcon(int32 CharacterNum);


public:
	// 메인 위젯이 두개이다. 평상시 위젯과, JRPG때의 위젯. 서로 다른 위젯이고 배틀일때 달라진다.

	void CameraPossess(FVector Location, FRotator Rotation);

	UFUNCTION(BlueprintCallable)
		void CameraSetUp(FVector Location);
	UFUNCTION(BlueprintCallable)
		void CameraRotSetUp(FRotator Rotation);
	void ExitCamera();
	FVector GetCameraLocation();

	void GameEndSpawnCharacter();

	void OpenESC();
	void MouseOnOff();

	void MouseOn();
	void MouseOff(); // 이렇게 따로 두는 이유는 필요에따라 계속해서 따로 On Off를 하기 위해서.


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

	void AddCharExp(int32 CharNum, float DropExp);
	//★★ 필드 에너미에서 캐릭터 번호를 가져와서 해당 캐릭터의 경험치를 증가시킨다.
	// 경험치가 다음 경험치를 넘기는 경우 레벨업을 시키고, 새롭게 스탯을 가져와야한다.


	void AddDropChar(int32 CharNum); // 만일 캐릭터를 얻을때의 경우 추가하는 함수. ★★
	

	// 배틀 
	void PlayBattleMode(TArray<FEnermys> EnermyUnits); // 배틀 시작.
	void ReturnMainWidget(); // 배틀 종료후 돌아가기.

	void StartBattleWidget();
	void BattleTurnStart(bool bFlag);
	void SetVisibleBattleWidget(bool bFlag); // 배틀위젯 숨기거나 켜기.
	void SetEnermyTurnWidget(bool bFlag); // 적 턴에서 필요한 위젯만 키기

	void BattleESC(); // 스킬 취소 및, 나가기 위젯 뛰우기
	void VisibleDamage(float Damage, FVector TargetLocation); // 데미지 띄우기


	void UnitTurnEnd(); // 턴엔드 유닛에서 블루프린트에서 해당 함수를 호출한다.

	void EnermyListSetup(); // 애님이 다 끝나고 턴 시작후에 카메라랑 락온을 변경해야한다.

	UFUNCTION(BlueprintCallable)
		void HiddenRockOn(); // 전체 스킬을 사용할때 락온 위젯이 가려지는 용도.


public:
	UPROPERTY(VisibleAnywhere)
		class AJRPGGameMode* GM;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		EGameModeType GameType;

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


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<int32> CurrentParty; // 현재 선택되어있는 파티리스트
	UPROPERTY(VisibleAnywhere)
		int32 CurrentFieldNum; // 필드 정보
	UPROPERTY(VisibleAnywhere)
		int32 CurrentPartyIndex = 0; // 현재 몇번째 인덱스인지.

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<int32> HaveCharList; // 가지고 있는 전체 캐릭터 넘버 (추후 세이브 로드 해야함).
	// 나중에 퀘스트 넘버처럼 번호와 BP 패스를 가지고 해당 넘버만 저장하고 불러오는 형식으로 만들어도될듯.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)	
		TMap<int32, int32> HaveCharLevels; // 가지고 있는 캐릭터의 레벨들. (레벨로 스탯을 데이터테이블에서 검색해서 가져옴)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TMap<int32, FJRPGCharStat> CharStats; // 가지고 있는 캐릭터의 스텟들.
	// 게임모드의 포스트 로그인에서 레벨로 데이터 테이블을 검색해서 스탯들을 미리 다 가져온다 .

	UPROPERTY(VisibleAnywhere)
		TMap<int32, float> CurrentExp; // 현재 경험치
	UPROPERTY(VisibleAnywhere)
		TMap<int32, float> NextExp; // 현재 경험치

	// ★★★ NextExp와 CharStats은 세이브를 하지않고, 게임 시작시 데이터 테이블에서 가져온 스탯값으로 저장한다.
	// 추후 값이 변경 되는 것을 우려해야하기 때문. 이미 저장되어있으면 데이터가 이상해진다.
	// 하지만 어처피 Add를 통해 키값을 저장해야하는 것을 해야하므로 세이브 자체는 해둔다.


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MainUI)
		TSubclassOf<class UJRPGTemaUI> BP_TemaMainUI;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MainUI)
		class UJRPGTemaUI* TemaMainUI;

	UPROPERTY(VisibleAnywhere)
		class UDataTable* UnitUITable;


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
	// AddToViewport 를 사용하는 UI만 저장한다. Animation을 가지고있는 이미 할당되어있는 UI는 제외.


	UPROPERTY(VisibleAnywhere)
		class AJRPGUnit* CurrentUnit; // ESC를 할 Unit 스킬을 실행 중이면 취소하고 , 아무것도 실행중이지 않으면 나가기 창을 띄운다.

	UPROPERTY(BlueprintReadWrite)
		class AJRPGUnit* TargetUnit; // BattleWidget에서 정한 현재 공격할 TargetUnit;
	UPROPERTY(BlueprintReadWrite)
		TArray<class AJRPGUnit*> TargetUnits;
	UFUNCTION(BlueprintCallable)
		void TargetToRotation();
	UFUNCTION(BlueprintCallable)
		void EnermyTargetToRotation();


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UDamageWidget> BP_DamageSkin;

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

	void BattleUIOnOff(bool bOnOff); // UI 켜기 끄기.
	void PlayPriority();

};
