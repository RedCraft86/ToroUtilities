// Copyright (C) RedCraft86. All Rights Reserved.

#include "Actors/Procedural/ToroSplineGenBase.h"

AToroSplineGenBase::AToroSplineGenBase(): bClosedLoop(false), SplineType(ESplinePointType::Curve)
{
	SplineComponent = CreateDefaultSubobject<USplineComponent>("SplineComponent");
	SplineComponent->AddSplinePoint(FVector(50.0f, 50.0f, 0.0f), ESplineCoordinateSpace::Local);
	SetRootComponent(SplineComponent);

	SceneRoot->SetupAttachment(SplineComponent);
}

#if WITH_EDITOR
void AToroSplineGenBase::OnConstruction(const FTransform& Transform)
{
	SplineComponent->SetClosedLoop(bClosedLoop);
	for (int i = 0; i < SplineComponent->GetNumberOfSplinePoints(); i++)
		SplineComponent->SetSplinePointType(i, SplineType);
		
	Super::OnConstruction(Transform);
}
#endif