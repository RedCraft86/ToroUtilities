// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "Libraries/ToroGeneralUtils.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/OutputDeviceNull.h"
#include "ShaderPipelineCache.h"
#include "Helpers/WorldGetter.h"

TArray<FVector> FActorBoundsCheckParams::ProcessVertices(const TArray<FVector>& InVertices, const FVector& Origin) const
{
	TArray<FVector> Out{Origin};
	for (const FVector& Vector : InVertices)
	{
		Out.AddUnique(FVector(
			FMath::Lerp(Origin.X, Vector.X, BoundingBoxLerp.X),
			FMath::Lerp(Origin.Y, Vector.Y, BoundingBoxLerp.Y),
			FMath::Lerp(Origin.Z, Vector.Z, BoundingBoxLerp.Z)
		));
	}
	return Out;
}

UWorld* UToroGeneralUtils::GetPlayWorld(const UObject* Context)
{
	return FWorldGetter::Get(Context);
}

int32 UToroGeneralUtils::GetNumCompilingShaders()
{
	return FShaderPipelineCache::NumPrecompilesRemaining();
}

TArray<FVector> UToroGeneralUtils::GetBoundingBoxVertices(const AActor* Target, const bool bOnlyCollidingComponents,
	const bool bIncludeFromChildActors, FVector& Origin, FVector& BoxExtent)
{
	TArray<FVector> Result;
	if (IsValid(Target))
	{
		Target->GetActorBounds(bOnlyCollidingComponents, Origin, BoxExtent, bIncludeFromChildActors);
	
		Result.AddUnique(Origin + BoxExtent);
		Result.AddUnique(Origin - BoxExtent);
		Result.AddUnique(FVector(Origin.X - BoxExtent.X, Origin.Y + BoxExtent.Y, Origin.Z + BoxExtent.Z));
		Result.AddUnique(FVector(Origin.X + BoxExtent.X, Origin.Y - BoxExtent.Y, Origin.Z + BoxExtent.Z));
		Result.AddUnique(FVector(Origin.X + BoxExtent.X, Origin.Y + BoxExtent.Y, Origin.Z - BoxExtent.Z));
		Result.AddUnique(FVector(Origin.X - BoxExtent.X, Origin.Y - BoxExtent.Y, Origin.Z + BoxExtent.Z));
		Result.AddUnique(FVector(Origin.X + BoxExtent.X, Origin.Y - BoxExtent.Y, Origin.Z - BoxExtent.Z));
		Result.AddUnique(FVector(Origin.X - BoxExtent.X, Origin.Y + BoxExtent.Y, Origin.Z - BoxExtent.Z));
	}
	return Result;
}

void UToroGeneralUtils::ForceGarbageCollection()
{
	if (GEngine)
	{
#if WITH_EDITOR
		GEngine->ForceGarbageCollection(false);
#else
		GEngine->ForceGarbageCollection(true);
#endif
	}
}

int32 UToroGeneralUtils::GetNumAsyncPkgs()
{
	return GetNumAsyncPackages();
}

float UToroGeneralUtils::GetAsyncPkgLoadPercent(const FString Package)
{
	return GetAsyncLoadPercentage(*Package);
}

void UToroGeneralUtils::CallLocalEvent(UObject* Target, const FName EventName)
{
	if (IsValid(Target) && !EventName.IsNone())
	{
		FOutputDeviceNull Ar;
		Target->CallFunctionByNameWithArguments(*EventName.ToString(),
			Ar, nullptr, true);
	}
}
