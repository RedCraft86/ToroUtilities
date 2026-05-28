// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "Actors/Editor/NavPathVisualizer.h"

ANavPathVisualizer::ANavPathVisualizer()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	bTickInEditor = false;

#if WITH_EDITOR && WITH_EDITORONLY_DATA
	EdShapes = CreateDefaultSubobject<UEdShapeComponent>(TEXT("EdShapes"));
#endif

#if WITH_EDITORONLY_DATA
	bIsMainWorldOnly = true;
	bIsSpatiallyLoaded = false;
	bRunConstructionScriptOnDrag = true;
#endif

	bEnableAutoLODGeneration = false; // Include Actor in HLOD option
	bIsEditorOnlyActor = true;

	SetCanBeDamaged(false);
}

#if WITH_EDITOR
void ANavPathVisualizer::BeginPlay()
{
	Super::BeginPlay();
	Destroy();
}

void ANavPathVisualizer::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
#if WITH_EDITORONLY_DATA
	if (EdShapes && !FApp::IsGame())
	{
		if (EdShapes->WireNavPaths.Contains(LastName))
		{
			EdShapes->WireNavPaths.Add(Name, EdShapes->WireNavPaths.FindRef(LastName));
			EdShapes->WireNavPaths.Remove(LastName);
			LastName = Name;
		}
		if (!EdShapes->WireNavPaths.Contains(Name))
		{
			EdShapes->WireNavPaths.Add(Name);
		}

		EdShapes->WireNavPaths[Name].Targets = Targets;
		EdShapes->WireNavPaths[Name].bRenderOnTop = bDrawOnTop;
	}
#endif
}
#endif
