// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#include "DataTypes/WireShapes.h"
#if WITH_EDITOR
#include "NavigationSystem.h"
#include "NavigationPath.h"
#endif

void FWireNavPathData::GeneratePathPoints(AActor* Owner)
{
#if WITH_EDITOR
	const int32 NumTargets = Targets.Num();
	if (NumTargets < 2 || FApp::IsGame())
	{
		PathPoints.Empty();
		return;
	}

	PathPoints.Empty(NumTargets);
	const FTransform& OwnerTransform = Owner->GetActorTransform();
	for (int32 i = 0; i < NumTargets - 1; i++)
	{
		const FVector Current = OwnerTransform.TransformPositionNoScale(Targets[i]);
		const FVector Next = OwnerTransform.TransformPositionNoScale(Targets[i + 1]);
		const UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(Owner, Current, Next);
		if (NavPath && NavPath->IsValid())
		{
			PathPoints.Append(NavPath->PathPoints);
		}
	}
#else
	PathPoints.Empty();
#endif
}

void FWireNavPathData::FindNearestPoint(const FVector& CameraPosition)
{
#if WITH_EDITOR
	if (PathPoints.IsEmpty() || FApp::IsGame())
	{
		NearestPoint.Reset();
		return;
	}

	NearestPoint = PathPoints[0];
	float LastDistSq = FVector::DistSquared(PathPoints[0], CameraPosition);
	for (int32 i = 1; i < PathPoints.Num(); i++)
	{
		const float DistSq = FVector::DistSquared(PathPoints[i], CameraPosition);
		if (DistSq < LastDistSq)
		{
			NearestPoint = PathPoints[i];
			LastDistSq = DistSq;
		}
	}
#else
	NearestPoint.Reset()
#endif
}
