// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

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