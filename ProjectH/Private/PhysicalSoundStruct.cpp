#include "PhysicalSoundStruct.h"
#include "Kismet/GameplayStatics.h"


EPhysicalSurface TracePysicalSurface(AActor* Unit, float EndDistance)
{
	if (Unit->GetWorld())
	{
		FHitResult HitResult;
		FVector Location = Unit->GetTargetLocation();
		FCollisionQueryParams Param;
		Param.bReturnPhysicalMaterial = true;
		Param.AddIgnoredActor(Unit);

		bool bHit = Unit->GetWorld()->LineTraceSingleByChannel(HitResult, Location, Location - FVector(0.f, 0.f, EndDistance), ECollisionChannel::ECC_GameTraceChannel11, Param);
		if (bHit)
		{
			return UGameplayStatics::GetSurfaceType(HitResult);
		}

	}
	
	return EPhysicalSurface::SurfaceType_Default;
}


