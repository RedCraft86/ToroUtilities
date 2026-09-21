// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#pragma once

#include "Engine/StaticMeshActor.h"
#include "ISMActors.generated.h"

/**
 * Wrapper for AStaticMeshActor that uses the Instanced Static Mesh Component.
 */
UCLASS(meta = (ChildCanTick = true))
class TORORUNTIME_API AInstancedStaticMeshActor : public AStaticMeshActor
{
	GENERATED_BODY()

public:

	AInstancedStaticMeshActor(const FObjectInitializer& ObjectInit);

#if WITH_EDITORONLY_DATA
	/**
	 * Instance transforms relative to the actor.
	 */
	UPROPERTY(Category = StaticMesh, EditAnywhere, meta = (MakeEditWidget, DisplayPriority = 1))
		TArray<FTransform> Instances = { FTransform::Identity };
#endif

	/**
	 * Gets the actor's static mesh component cast to the requested component type.
	 * @tparam T Expected instanced static mesh component type.
	 */
	template<typename T = UInstancedStaticMeshComponent>
	T* GetMeshComponent() const
	{
		return Cast<T>(GetStaticMeshComponent());
	}

#if WITH_EDITOR
	/**
	 * Rebuilds the component instances from the editable transform array.
	 */
	void UpdateInstances() const;

	/**
	 * Copies the component's current instance transforms into the editable array.
	 */
	void CopyInstancesFromComponent();

protected:

	/**
	 * Lock the component's instances from being updated by this actor.
	 */
	bool bLockInstances = false;

	virtual void OnConstruction(const FTransform& Transform) override;
#endif
};

/**
 * Wrapper for AStaticMeshActor that uses the Hierarchical Instanced Static Mesh Component.
 */
UCLASS(meta = (ChildCanTick = true))
class TORORUNTIME_API AHierarchicalInstancedStaticMeshActor : public AInstancedStaticMeshActor
{
	GENERATED_BODY()

public:

	AHierarchicalInstancedStaticMeshActor(const FObjectInitializer& ObjectInit);
};