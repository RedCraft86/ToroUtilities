// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#pragma once

#include "Engine/StaticMeshActor.h"
#include "ISMActors.generated.h"

/**
 * An optimized Actor for rendering large numbers of the same Static Mesh asset.
 * This class replaces the standard Static Mesh Component with an Instanced version (ISM).
 */
UCLASS(meta = (ChildCanTick = true))
class TOROCORE_API AInstancedStaticMeshActor : public AStaticMeshActor
{
	GENERATED_BODY()

public:

	AInstancedStaticMeshActor(const FObjectInitializer& ObjectInit);

	template<typename T = UInstancedStaticMeshComponent>
	T* GetMeshComponent() const { return Cast<T>(GetStaticMeshComponent()); }

#if WITH_EDITORONLY_DATA
	/**
	* The master list of transforms for all instances.
	*/
	UPROPERTY(Category = StaticMesh, EditAnywhere, meta = (MakeEditWidget, DisplayPriority = 1))
		TArray<FTransform> Instances = { FTransform::Identity };
#endif

#if WITH_EDITOR
	void UpdateInstances() const;
	void CopyInstancesFromComponent();

protected:

	bool bLockInstances = false;

	virtual void OnConstruction(const FTransform& Transform) override;
#endif
};

/**
 * An extension of AInstancedStaticMeshActor that utilizes HISMs.
 * Recommended for meshes that require LOD (Level of Detail) support and
 * occlusion culling for high-density environments.
 */
UCLASS(meta = (ChildCanTick = true))
class TOROCORE_API AHierarchicalInstancedStaticMeshActor : public AInstancedStaticMeshActor
{
	GENERATED_BODY()

public:

	AHierarchicalInstancedStaticMeshActor(const FObjectInitializer& ObjectInit);
};
