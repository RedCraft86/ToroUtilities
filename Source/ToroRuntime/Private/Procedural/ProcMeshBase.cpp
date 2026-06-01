// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "Procedural/ProcMeshBase.h"

AProcMeshBase::AProcMeshBase()
{
	MeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->bAffectDynamicIndirectLighting = false;
	MeshComponent->bUseComplexAsSimpleCollision = false;
	MeshComponent->SetupAttachment(SceneRoot);
}

#if WITH_EDITOR
void AProcMeshBase::BakeActor()
{
	AActor* NewActor = FActorBaking::BakeProceduralMesh(MeshComponent);
	NewActor->SetActorScale3D(GetActorScale3D());
	NewActor->SetFolderPath(GetFolderPath());
	NewActor->SetActorLabel(GetActorLabel());
	if (bBakeDestroySource)
	{
		FActorBaking::DestroyActors({this});
	}
}
#endif