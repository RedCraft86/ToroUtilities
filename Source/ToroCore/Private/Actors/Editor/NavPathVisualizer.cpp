// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#include "Actors/Editor/NavPathVisualizer.h"
#if WITH_EDITOR
#include "Components/Editor/WireShapeComponent.h"
#endif

ANavPathVisualizer::ANavPathVisualizer()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
#if WITH_EDITORONLY_DATA
	SceneRoot->bVisualizeComponent = true;
#endif
	SetRootComponent(SceneRoot);

#if WITH_EDITOR
	ShapeComponent = CreateEditorOnlyDefaultSubobject<UWireShapeComponent>(TEXT("WireShapes"));
#endif

#if WITH_EDITORONLY_DATA
	bIsMainWorldOnly = true;
	bIsSpatiallyLoaded = false;
	bRunConstructionScriptOnDrag = true;
#endif

	bEnableAutoLODGeneration = false; // Exclude this actor from HLOD assuming it is gameplay specific
	bIsEditorOnlyActor = true;

	SetCanBeDamaged(false);
}

#if WITH_EDITOR
void ANavPathVisualizer::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimerForNextTick([WeakThis = TWeakObjectPtr(this)]()
	{
		if (WeakThis.IsValid())
		{
			WeakThis->Destroy();
		}
	});
}

void ANavPathVisualizer::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (!ShapeComponent || FApp::IsGame())
	{
		return;
	}

	ShapeComponent->WireNavPaths.Empty(1);
	FWireNavPathData& Data = ShapeComponent->WireNavPaths.Add(*GetActorLabel());
	Data.Targets = PathTargets;
}
#endif