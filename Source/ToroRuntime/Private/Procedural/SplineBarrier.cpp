// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#include "Procedural/SplineBarrier.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/Material.h"

ASplineBarrier::ASplineBarrier()
{
	SceneRoot->SetMobility(EComponentMobility::Static);
	SplineComponent->SetMobility(EComponentMobility::Static);

	WallMeshes = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("WallMeshes"));
	WallMeshes->bAffectDynamicIndirectLighting = false;
	WallMeshes->bAffectDistanceFieldLighting = false;
	WallMeshes->SetMobility(EComponentMobility::Static);
	WallMeshes->SetLightingChannels(false, false, false);
	WallMeshes->SetGenerateOverlapEvents(false);
	WallMeshes->SetCastShadow(false);
	WallMeshes->SetupAttachment(SceneRoot);

	bClosedLoop = true;
	bRestrictSplineType = true;
	SplineType = ESplinePointType::Linear;

	WallHeight = 2.0f;
	Collision.SetProfileName(TEXT("InvisibleWall"));

	bIsEditorOnlyActor = false;
	bRealtimeConstruction = true;

	if (!IsRunningCommandlet() && !IsRunningDedicatedServer())
	{
		static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshFinder(
		   TEXT("/ToroUtilities/Procedural/SM_Barrier.SM_Barrier"));
		if (MeshFinder.Succeeded())
		{
			WallMesh = MeshFinder.Object;
			WallMeshes->SetStaticMesh(WallMesh);
		}

		static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialFinder(
			TEXT("/ToroUtilities/Procedural/M_Wireframe.M_Wireframe"));
		if (MaterialFinder.Succeeded())
		{
			OverlayMaterial = MaterialFinder.Object;
			WallMeshes->SetOverlayMaterial(OverlayMaterial);
		}
	}

	SetHidden(true);
}

#if WITH_EDITOR
void ASplineBarrier::Construct()
{
	WallMeshes->ClearInstances();
	const int NumPoints = SplineComponent->GetNumberOfSplinePoints();
	if (NumPoints < 2)
	{
		return;
	}

	const int32 SegmentCount = NumPoints - (SplineComponent->IsClosedLoop() ? 0 : 1);
	for (int32 Segment = 0; Segment < SegmentCount; Segment++)
	{
		if (!SkipSegments.Contains(Segment))
		{
			FVector Location, Tangent;
			SplineComponent->GetLocalLocationAndTangentAtSplinePoint(Segment, Location, Tangent);
			const float DistB = SplineComponent->GetDistanceAlongSplineAtSplinePoint(Segment + 1);
			const float DistA = SplineComponent->GetDistanceAlongSplineAtSplinePoint(Segment);

			WallMeshes->AddInstance({
				Tangent.Rotation(), Location,
				FVector((DistB - DistA) / 100.0f, 1.0f, WallHeight)
			}, false);
		}
	}

	Super::Construct();
}

void ASplineBarrier::OnConstruction(const FTransform& Transform)
{
	if (ensureMsgf(WallMesh, TEXT("WallMesh was not found!")))
	{
		WallMeshes->SetStaticMesh(WallMesh);
		WallMeshes->SetOverlayMaterial(OverlayMaterial);
	}

	Collision.ToPrimitiveComponent(WallMeshes);

	bRestrictSplineType = true;
	SplineType = ESplinePointType::Linear;

	Super::OnConstruction(Transform);
}

bool ASplineBarrier::CanEditChange(const FProperty* InProperty) const
{
	return InProperty
		&& InProperty->GetName() != GET_MEMBER_NAME_CHECKED(ASplineBarrier, SplineType)
		&& Super::CanEditChange(InProperty);
}
#endif