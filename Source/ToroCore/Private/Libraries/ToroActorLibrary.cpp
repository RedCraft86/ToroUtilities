// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#include "Libraries/ToroActorLibrary.h"

void UToroActorLibrary::GetBoundingBoxVertices(TSet<FVector>& OutVerts, FVector& Origin, FVector& Extent,
	const AActor* Target, const FVector Scale, const bool bOnlyColliding, const bool bChildActors)
{
	OutVerts.Empty(8);
	Origin = FVector::ZeroVector;
	Extent = FVector::ZeroVector;

	if (IsValid(Target))
	{
		Target->GetActorBounds(bOnlyColliding, Origin, Extent, bChildActors);
		if (Scale.Size() < 0.05f)
		{
			OutVerts.Add(Origin);
			return;
		}

		//     (+Z)
		//      |
		//      |
		//      +--------(+Y)
		//     /
		//    /
		//  (+X)
		//
		//    (5)---------------(4)
		//    /|                /|
		//   / |               / |
		// (6)---------------(7) |
		//  |  |              |  |
		//  |  |     (O)      |  |
		//  | (0)-------------|-(3)
		//  | /               | /
		//  |/                |/
		// (1)---------------(2)
		//
		// O = Origin
		// 0..7 = Extent Multipliers (Index)
		const FVector Multipliers[8] {
			FVector(-1, -1, -1), // A
			FVector( 1, -1, -1), // B
			FVector( 1,  1, -1), // C
			FVector(-1,  1, -1), // D
			FVector(-1,  1,  1), // E
			FVector(-1, -1,  1), // F
			FVector( 1, -1,  1), // G
			FVector( 1,  1,  1)  // H
		};

		for (int i = 0; i < 8; i++)
		{
			OutVerts.Add(Origin + (Extent * Multipliers[i] * Scale));
		}
	}
}

void UToroActorLibrary::AddActorTag(AActor* Target, const FName InTag)
{
	if (IsValid(Target))
	{
		Target->Tags.AddUnique(InTag);
	}
}

void UToroActorLibrary::RemoveActorTag(AActor* Target, const FName InTag)
{
	if (IsValid(Target))
	{
		Target->Tags.Remove(InTag);
	}
}
