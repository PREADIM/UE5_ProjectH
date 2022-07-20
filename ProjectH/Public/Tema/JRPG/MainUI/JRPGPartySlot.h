// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Blueprint/UserWidget.h"
#include "JRPGPartySlot.generated.h"

/**
 * 
 */
// 메인 위젯의 파티 리스트를 표현하기위한 하나의 슬롯. 
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
	void Init(UTexture2D* tex, FString Name, int32 Num); // 이름이랑 아이콘 설정.
	
};
