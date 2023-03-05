#pragma once

#include "ProjectH.h"
#include "Engine/DataTable.h"
#include "Widgets/Layout/Anchors.h"
#include "QTEWidgetPosition.generated.h"



USTRUCT(BlueprintType)
struct FQTEWidgetPosition
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FAnchors Anchor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector2D Alignment = FVector2D(0.f, 0.f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector2D Position = FVector2D(0.f, 0.f);
};

