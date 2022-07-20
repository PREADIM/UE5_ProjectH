// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Blueprint/UserWidget.h"
#include "JRPGPartySlot.generated.h"

/**
 * 
 */
// ���� ������ ��Ƽ ����Ʈ�� ǥ���ϱ����� �ϳ��� ����. 
UCLASS()
class PROJECTH_API UJRPGPartySlot : public UCustomWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UImage* UnitProfile;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* UnitName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* KeyText;


public:
	void Init(UTexture2D* tex, FString Name, int32 Num); // �̸��̶� ������ ����.
	
};
