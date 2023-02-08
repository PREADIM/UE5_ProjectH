#pragma once
#include "ProjectH.h"
#include "PhysicalSoundStruct.generated.h"


USTRUCT(BlueprintType)
struct FSoundAndAttenuation
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundBase* Sound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundAttenuation* Attenuation;
};


USTRUCT(BlueprintType)
struct FPhysicalSoundStruct
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<class USoundBase*> Sounds;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundAttenuation* Attenuation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TEnumAsByte<EPhysicalSurface> SurfaceName;
};

EPhysicalSurface TracePysicalSurface(AActor* Unit, float EndDistance);

