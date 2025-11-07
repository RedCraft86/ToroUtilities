// Copyright (C) RedCraft86. All Rights Reserved.

#include "Actors/Procedural/SplineCable.h"

void ASplineCable::DestroyComponents()
{
	for (const TObjectPtr<USplineMeshComponent>& Comp : SplineMeshComponents)
	{
		if (Comp) Comp->DestroyComponent();
	}

	for (const TObjectPtr<UInstancedStaticMeshComponent>& Comp : AttachMeshComponents)
	{
		if (Comp) Comp->DestroyComponent();
	}

	if (StartMeshComponent) 
	{
		StartMeshComponent->DestroyComponent();
		StartMeshComponent = nullptr;
	}
    
	if (EndMeshComponent) 
	{
		EndMeshComponent->DestroyComponent();
		EndMeshComponent = nullptr;
	}

	SplineMeshComponents.Empty();
	AttachMeshComponents.Empty(AttachmentCount);
}

void ASplineCable::Construct()
{
	DestroyComponents();

	const int NumPoints = SplineComponent->GetNumberOfSplinePoints();
	if (NumPoints < 2)
	{
		return;
	}

	if (CableMesh.IsValidData())
	{
		const int32 SegmentCount = NumPoints - (bClosedLoop ? 0 : 1);
		for (int32 i = 0; i < SegmentCount; i++)
		{
			const int32 NextIdx = (i + 1) % NumPoints;
			if (USplineMeshComponent* MeshComp = AddGenericComponent<USplineMeshComponent>())
			{
				CableMesh.ToStaticMeshComponent(MeshComp);
				Collision.ToPrimitiveComponent(MeshComp);
				MeshComp->SetCanEverAffectNavigation(false);

				FVector Location, Tangent, NextLocation, NextTangent;
				SplineComponent->GetLocalLocationAndTangentAtSplinePoint(i, Location, Tangent);
				SplineComponent->GetLocalLocationAndTangentAtSplinePoint(NextIdx, NextLocation, NextTangent);

				MeshComp->SetForwardAxis(CableMesh.MeshAxis, false);
				MeshComp->SetStartAndEnd(Location, Tangent,
					NextLocation, NextTangent, true);

				SplineMeshComponents.Add(MeshComp);
			}
		}
	}

	if (!bClosedLoop)
	{
		if (StartCap.IsValidData())
		{
			if (StartMeshComponent = AddGenericComponent<UStaticMeshComponent>(); StartMeshComponent)
			{
				StartCap.ToStaticMeshComponent(StartMeshComponent);
				Collision.ToPrimitiveComponent(StartMeshComponent);
				StartMeshComponent->SetCanEverAffectNavigation(false);
	
				FVector Location, Tangent;
				SplineComponent->GetLocalLocationAndTangentAtSplinePoint(0, Location, Tangent);
			
				StartMeshComponent->SetRelativeTransform({
					StartCap.Transform.GetRotation().Rotator() + FRotationMatrix::MakeFromX(Tangent).Rotator(),
					StartCap.Transform.GetTranslation() + Location,
					StartCap.Transform.GetScale3D()
				});
			}
		}

		if (EndCap.IsValidData())
		{
			if (EndMeshComponent = AddGenericComponent<UStaticMeshComponent>(); EndMeshComponent)
			{
				EndCap.ToStaticMeshComponent(EndMeshComponent);
				Collision.ToPrimitiveComponent(EndMeshComponent);
				EndMeshComponent->SetCanEverAffectNavigation(false);
			
				FVector Location, Tangent;
				SplineComponent->GetLocalLocationAndTangentAtSplinePoint(NumPoints - 1, Location, Tangent);
					
				EndMeshComponent->SetRelativeTransform({
					EndCap.Transform.GetRotation().Rotator() + FRotationMatrix::MakeFromX(Tangent).Rotator(),
					EndCap.Transform.GetTranslation() + Location,
					EndCap.Transform.GetScale3D()
				});
			}
		}
	}

	if (AttachmentCount > 0 && !AttachmentSample.IsEmpty())
	{
		for (int i = 0; i < AttachmentSample.Num(); i++)
		{
			UInstancedStaticMeshComponent* MeshComp = AddGenericComponent<UInstancedStaticMeshComponent>();
			AttachmentSample[i].ToStaticMeshComponent(MeshComp);
			Collision.ToPrimitiveComponent(MeshComp);
			MeshComp->SetCanEverAffectNavigation(false);
			AttachMeshComponents.Add(MeshComp);
		}

		if (AttachmentIndices.Num() != AttachmentCount)
		{
			AttachmentIndices = LoopMeshArray(AttachmentSample, AttachmentLoopMode, AttachmentCount);
		}

		const float Interval = SplineComponent->GetSplineLength() / (AttachmentCount + 1);
		for (int32 i = 0; i < AttachmentIndices.Num(); i++)
		{
			const int32 idx = AttachmentIndices[i];
			if (!AttachmentSample.IsValidIndex(idx)
				|| !AttachMeshComponents.IsValidIndex(idx))
				continue;

			const FTransform Placement = SplineComponent->GetTransformAtDistanceAlongSpline(
				(i + 1) * Interval, ESplineCoordinateSpace::Local);

			const FTransform& Transform = AttachmentSample[idx].Transform;
			AttachMeshComponents[idx]->AddInstance({
				Transform.GetRotation() * Placement.GetRotation(),
				Transform.GetTranslation() + Placement.GetTranslation(),
				Transform.GetScale3D()
			});
		}
	}
	
	Super::Construct();
}

#if WITH_EDITOR
void ASplineCable::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass, AttachmentSample)
		|| PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass, AttachmentLoopMode)
		|| PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass, AttachmentCount))
	{
		AttachmentIndices = LoopMeshArray(AttachmentSample, AttachmentLoopMode, AttachmentCount);
		Construct();
	}
}
#endif
