// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "GameFramework/PlayerController.h"
#include "Tema/JRPG/JRPGCharStat.h"
#include "JRPGPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API AJRPGPlayerController : public APlayerController
{
	GENERATED_BODY()


// ������ ���۵Ǹ� Ư�� ������ �̵��ؼ� �ش� �������� ������ �ϰ��Ѵ�. ��, ���ƴٴϴ� ���� �����͸� �������ʰ�, 
// �׳� ���� �ʿ��� ĳ���͸� �����ؼ� ���� ���̴�.


public:
	virtual void BeginPlay();
	virtual void OnPossess(APawn* NewPawn) override;


public:
	// ���� ������ �ΰ��̴�. ���� ������, JRPG���� ����. ���� �ٸ� �����̰� ��Ʋ�϶� �޶�����.

	void CameraPossess(FVector Location);
	void CameraSetUp(FVector Location);
	void ExitCamera();
	void PlayBattleMode(TArray<int32> EnermyUnits);

	void GameEndSpawnCharacter();





public:
	UPROPERTY(VisibleAnywhere)
		class AJRPGGameMode* GM;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AJRPGCamera> BP_Camera;
	class AJRPGCamera* DynamicCamera;


	UPROPERTY()
		FTransform FieldLocation;
	UPROPERTY()
		int32 RepreCharacterNum; // ��ǥĳ����
	UPROPERTY()
		class AJRPGUnit* RepreCharacter;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<int32> CurrentParty; // ���� ���õǾ��ִ� ��Ƽ����Ʈ
	UPROPERTY(VisibleAnywhere)
		int32 CurrentFieldNum; // �ʵ� ����


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<int32> HaveCharList; // ������ �ִ� ��ü ĳ���� �ѹ� (���� ���̺� �ε� �ؾ���).
	// ���߿� ����Ʈ �ѹ�ó�� ��ȣ�� BP �н��� ������ �ش� �ѹ��� �����ϰ� �ҷ����� �������� �����ɵ�.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TMap<int32, FJRPGCharStat> HaveCharStat; // ������ �ִ� ĳ������ ���ݵ�.
};
