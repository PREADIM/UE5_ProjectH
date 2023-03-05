#pragma once

#include "ProjectH.h"
#include "Engine/DataTable.h"
#include "CameraTransition.generated.h"


USTRUCT(BlueprintType)
struct FCameraTransition
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class AQTECamera* NewCamera = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float BlendTime = 0.f;

};

