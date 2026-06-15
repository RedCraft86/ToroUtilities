// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "Engine/StaticMeshActor.h"
#include "ISMActors.generated.h"

/**
 * An optimized Actor for rendering large numbers of the same Static Mesh asset.
 * This class replaces the standard Static Mesh Component with an Instanced version (ISM).
 * 
 * Key Feature:
 * Provides an editor-only 'Instances' array. Each element in this array is linked 
 * to a 3D widget in the viewport (via MakeEditWidget), allowing designers to 
 * visually layout hundreds of instances without the overhead of individual Actors.
 */
UCLASS(meta = (ChildCanTick = true))
class TOROCORE_API AInstancedStaticMeshActor : public AStaticMeshActor
{
	GENERATED_BODY()

public:

	AInstancedStaticMeshActor(const FObjectInitializer& ObjectInit);

	template<typename T>
	T* GetMeshComponent() const { return Cast<T>(GetStaticMeshComponent()); }

#if WITH_EDITORONLY_DATA
	/** 
	* The master list of transforms for all instances.
	* The 'MakeEditWidget' metadata spawns a 3D transform gizmo for every element 
	* in the viewport, enabling rapid manual layout.
	*/
	UPROPERTY(Category = StaticMesh, EditAnywhere, meta = (MakeEditWidget, DisplayPriority = 1))
		TArray<FTransform> Instances = { FTransform::Identity };
#endif

#if WITH_EDITOR
	void UpdateInstances() const;
	void CopyInstancesFromComponent();

protected:

	// Used during copying from component to instances array
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
