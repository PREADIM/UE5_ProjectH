#pragma once
#include "ProjectH.h"
#include "PhysicalSoundStruct.generated.h"


USTRUCT(BlueprintType)
struct FSoundAndAttenuation
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundBase* Sound = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundAttenuation* Attenuation = nullptr;
};


USTRUCT(BlueprintType)
struct FPhysicalSoundStruct
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<class USoundBase*> Sounds;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundAttenuation* Attenuation = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TEnumAsByte<EPhysicalSurface> SurfaceName = EPhysicalSurface::SurfaceType_Default;
};

EPhysicalSurface TracePysicalSurface(AActor* Unit, float EndDistance);

