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

void FWireNavPathData::FindNearestPoint(const FTransform& Camera)
{
	LabelPoint.Reset();
#if WITH_EDITOR
	if (PathPoints.IsEmpty() || FApp::IsGame())
	{
		return;
	}

	constexpr float DotWeight = 0.5f;
	constexpr float DistWeight = 1.0f - DotWeight;

	TMap<int32, float> Weights;
	const FVector CameraLoc = Camera.GetTranslation();
	const FVector CameraFwd = Camera.GetRotation().GetForwardVector();
	for (int32 i = 0; i < PathPoints.Num(); i++)
	{
		float& Weight = Weights.Add(i, 1.0f);

		const FVector ToPoint = PathPoints[i] - CameraLoc;
		Weight += FMath::Max(0.0f, FVector::DotProduct(ToPoint.GetSafeNormal(), CameraFwd)) * DotWeight;

		const float DistSq = FVector::DistSquared(PathPoints[i], CameraLoc);
		Weight += (1.0f - FMath::Clamp(DistSq / FMath::Square(1000.0f), 0.0f, 1.0f)) * DistWeight;
	}

	float BestWeight = 0.0f;
	int32 BestIndex = INDEX_NONE;
	for (const TPair<int32, float>& IdxToWeight : Weights)
	{
		if (IdxToWeight.Value > BestWeight)
		{
			BestWeight = IdxToWeight.Value;
			BestIndex = IdxToWeight.Key;
		}
	}

	if (BestIndex != INDEX_NONE && PathPoints.IsValidIndex(BestIndex))
	{
		LabelPoint = PathPoints[BestIndex];
	}
#endif
}
