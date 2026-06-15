// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#include "Actors/ISMActors.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

AInstancedStaticMeshActor::AInstancedStaticMeshActor(const FObjectInitializer& ObjectInit)
	: Super(ObjectInit.SetDefaultSubobjectClass<UInstancedStaticMeshComponent>(StaticMeshComponentName))
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	bCanBeInCluster = GetClass() == AInstancedStaticMeshActor::StaticClass();

	if (!IsRunningCommandlet() && !IsRunningDedicatedServer())
	{
		if (UStaticMeshComponent* MeshComp = GetStaticMeshComponent())
		{
			static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshFinder(
				TEXT("/Engine/BasicShapes/Cube.Cube"));
			if (MeshFinder.Succeeded()) MeshComp->SetStaticMesh(MeshFinder.Object);

			static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialFinder(
				TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));
			if (MaterialFinder.Succeeded()) MeshComp->SetMaterial(0, MaterialFinder.Object);
		}
	}
}

#if WITH_EDITOR
void AInstancedStaticMeshActor::UpdateInstances() const
{
	UInstancedStaticMeshComponent* ISMC = GetMeshComponent<UInstancedStaticMeshComponent>();
	if (ISMC && !bLockInstances)
	{
		ISMC->ClearInstances();
		ISMC->AddInstances(Instances, false);
	}
}

void AInstancedStaticMeshActor::CopyInstancesFromComponent()
{
	bLockInstances = true;
	if (const UInstancedStaticMeshComponent* ISMC = GetMeshComponent<UInstancedStaticMeshComponent>())
	{
		const int32 NumInstances = ISMC->GetNumInstances();
		Instances.Empty(NumInstances);

		for (int32 i = 0; i < NumInstances; i++)
		{
			FTransform T;
			ISMC->GetInstanceTransform(i, T, false);
			Instances.Add(T);
		}
	}
	bLockInstances = false;
}

void AInstancedStaticMeshActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (Instances.IsEmpty())
	{
		Instances.Add(FTransform::Identity);
	}
	UpdateInstances();
}
#endif

AHierarchicalInstancedStaticMeshActor::AHierarchicalInstancedStaticMeshActor(const FObjectInitializer& ObjectInit)
	: Super(ObjectInit.SetDefaultSubobjectClass<UHierarchicalInstancedStaticMeshComponent>(StaticMeshComponentName))
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	bCanBeInCluster = GetClass() == AHierarchicalInstancedStaticMeshActor::StaticClass();
}
