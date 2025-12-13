// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "Actors/Procedural/SplineBarrier.h"
#if WITH_EDITOR
#include "Subsystems/EditorActorSubsystem.h"
#include "Actors/ISMActor.h"
#endif

ASplineBarrier::ASplineBarrier(): WallHeight(2.0f)
{
	SplineComponent->SetMobility(EComponentMobility::Static);
	SceneRoot->SetMobility(EComponentMobility::Static);

	WallMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>("WallMeshComponent");
	WallMeshComponent->SetMobility(EComponentMobility::Static);
	WallMeshComponent->SetupAttachment(GetRootComponent());
	WallMeshComponent->SetLightingChannels(false, false, false);
	WallMeshComponent->bAffectDynamicIndirectLighting = false;
	WallMeshComponent->bAffectDistanceFieldLighting = false;
	WallMeshComponent->SetGenerateOverlapEvents(false);
	WallMeshComponent->SetCastShadow(false);

	SetHidden(true);

	bClosedLoop = true;
	SplineType = ESplinePointType::Linear;
	Collision.SetProfileName(TEXT("InvisibleWall"));
	
#if WITH_EDITOR
	LoadObjects();
#endif
}

#if WITH_EDITORONLY_DATA
void ASplineBarrier::BakeInstances()
{
#if WITH_EDITOR
	const FScopedTransaction Transaction(NSLOCTEXT("ToroCore", "BakeSplineBarrier", "Bake Spline Barrier"));

	UEditorActorSubsystem* Subsystem = GEditor ? GEditor->GetEditorSubsystem<UEditorActorSubsystem>() : nullptr;
	if (!Subsystem) return;

	if (AInstancedStaticMeshActor* ISMA = Cast<AInstancedStaticMeshActor>(Subsystem->SpawnActorFromClass(
		AInstancedStaticMeshActor::StaticClass(), GetActorLocation(), GetActorRotation())))
	{
		ISMA->bPauseConstruction = true;
		ISMA->SetActorHiddenInGame(true);
		UpdateInstances(ISMA->GetMeshComponent<UInstancedStaticMeshComponent>());
		ISMA->CopyInstancesFromComponent();
		ISMA->bPauseConstruction = false;

		ISMA->SetActorLabel(GetActorLabel() + TEXT("_BAKED"));
		ISMA->SetFolderPath(GetFolderPath());

		Subsystem->SetActorSelectionState(ISMA, true);
		if (bBakeRemoveSource)
		{
			Subsystem->SetActorSelectionState(this, false);
			Subsystem->DestroyActor(this);
		}
	}
#endif
}
#endif

#if WITH_EDITOR
void ASplineBarrier::LoadObjects()
{
	if (!Mesh) Mesh = LoadObject<UStaticMesh>(nullptr,
		TEXT("/ToroUtilities/Assets/Procedural/SM_ProcBarrier.SM_ProcBarrier"));

	if (!Material) Material = LoadObject<UMaterialInterface>(nullptr,
		TEXT("/ToroUtilities/Assets/Procedural/M_ProcBarrier_01.M_ProcBarrier_01"));

	if (!OverlayMaterial) OverlayMaterial = LoadObject<UMaterialInterface>(nullptr,
		TEXT("/ToroUtilities/Assets/Procedural/M_ProcBarrier_02.M_ProcBarrier_02"));
}

bool ASplineBarrier::CanEditChange(const FProperty* InProperty) const
{
	return InProperty && InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(ThisClass, SplineType)
		? false : Super::CanEditChange(InProperty);
}
#endif

void ASplineBarrier::UpdateInstances(UInstancedStaticMeshComponent* Comp) const
{
	Comp->ClearInstances();
	Comp->SetStaticMesh(Mesh);
	Comp->SetMaterial(0, Material);
	Collision.ToPrimitiveComponent(Comp);

	const int NumPoints = SplineComponent->GetNumberOfSplinePoints();
	if (NumPoints < 2)
	{
		return;
	}
	
	const int32 SegmentCount = NumPoints - (bClosedLoop ? 0 : 1);
	for (int32 i = 0; i < SegmentCount; i++)
	{
		if (SkipIndexes.Contains(i))
			continue;
		
		FVector Location, Tangent;
		SplineComponent->GetLocalLocationAndTangentAtSplinePoint(i, Location, Tangent);
		const float DistB = SplineComponent->GetDistanceAlongSplineAtSplinePoint(i + 1);
		const float DistA = SplineComponent->GetDistanceAlongSplineAtSplinePoint(i);

		Comp->AddInstance({
			Tangent.Rotation(), Location,
			FVector((DistB - DistA) / 100.0f, 1.0f, WallHeight)
		}, false);
	}
	
#if WITH_EDITOR
	Comp->SetOverlayMaterial(OverlayMaterial);
#endif
}

void ASplineBarrier::BeginPlay()
{
	Super::BeginPlay();
	WallMeshComponent->SetOverlayMaterial(nullptr);
}

void ASplineBarrier::Construct()
{
#if WITH_EDITOR
	LoadObjects();
#endif
	UpdateInstances(WallMeshComponent);
	Super::Construct();
}
