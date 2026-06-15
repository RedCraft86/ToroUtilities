// Copyright (C) 2026 RedCraft86. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#include "Procedural/ProcSplineBase.h"

AProcSplineBase::AProcSplineBase()
	: bClosedLoop(false), bRestrictSplineType(false), SplineType(ESplinePointType::Curve)
{
	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	SplineComponent->AddSplinePoint(FVector(50.0f, 50.0f, 0.0f), ESplineCoordinateSpace::Local);
	SplineComponent->SetupAttachment(SceneRoot);
}

#if WITH_EDITOR
void AProcSplineBase::OnConstruction(const FTransform& Transform)
{
	SplineComponent->SetClosedLoop(bClosedLoop);
	if (bRestrictSplineType)
	{
		for (int i = 0; i < SplineComponent->GetNumberOfSplinePoints(); i++)
		{
			if (SplineComponent->GetSplinePointType(i) != SplineType)
			{
				SplineComponent->SetSplinePointType(i, SplineType);
			}
		}
	}

	Super::OnConstruction(Transform);
}
#endif