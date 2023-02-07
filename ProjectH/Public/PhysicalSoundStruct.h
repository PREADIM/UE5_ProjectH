#pragma once
#include "ProjectH.h"
#include "PhysicalSoundStruct.generated.h"

USTRUCT(BlueprintType)
struct FPhysicalSoundStruct
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<class USoundBase*> PhysicalSounds;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TEnumAsByte<EPhysicalSurface> SurfaceName;
};

EPhysicalSurface TracePysicalSurface(AActor* Unit, float EndDistance);

