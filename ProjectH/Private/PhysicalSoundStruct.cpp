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
			//DrawDebugLine(Unit->GetWorld(), Location, Location - FVector(0.f, 0.f, EndDistance), FColor::Blue);
			//DrawDebugPoint(Unit->GetWorld(), HitResult.Location, 10.f, FColor::Red, false, 1.f);
			////_DEBUG("%s", *UEnum::GetValueAsString(TempSurface));
			return UGameplayStatics::GetSurfaceType(HitResult);
		}

	}
	 
	//_DEBUG("Not Surface");
	return EPhysicalSurface::SurfaceType_Default;
}


