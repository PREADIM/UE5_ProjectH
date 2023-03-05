#pragma once

#include "ProjectH.h"
#include "Engine/DataTable.h"
#include "Special/QTE/QTEWidgetPosition.h"
#include "KeyEventInfo.generated.h"

UENUM(BlueprintType)
enum class EQTEState : uint8
{
	NONE		UMETA(DisplayName = "NONE"),
	Pending		UMETA(DisplayName = "Pending"),
	Success		UMETA(DisplayName = "Success"),
	FailTime	UMETA(DisplayName = "FailTime"),
	FailKey		UMETA(DisplayName = "FailKey")
};


USTRUCT(BlueprintType)
struct FKeyEventInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EQTEState State = EQTEState::NONE;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FKey Key;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FQTEWidgetPosition ScreenPosition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float TotalTime = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ExpiredTime = 0.f;


	void SetValues(EQTEState St, FKey K, FQTEWidgetPosition SP, float TotalTime, float ExpiredTime = 0.0f);
};

