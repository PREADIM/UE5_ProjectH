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
		FVector2D Alignment;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector2D Position;
};

