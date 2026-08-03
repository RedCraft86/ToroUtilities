// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#pragma once

#if WITH_EDITOR
#include "DataTypes/CachedGetter.h"
#include "DataTypes/MeshProperties.h"
#include "ProceduralMeshComponent.h"
#include "Subsystems/EditorActorSubsystem.h"
#endif

/**
 * A static utility class designed to split a large set of actors into
 * standalone static meshes or merge them into instanced static meshes.
 *
 * <b>All functions are Editor-Only.</b>
 */
class TOROCORE_API FActorBaking final
{
#if WITH_EDITOR
public:

	/**
	 * Cached accessor for the Editor Actor Subsystem.
	 */
	static inline TCachedGetter<UEditorActorSubsystem> ActorSubsystem{[] {
		return GEditor->GetEditorSubsystem<UEditorActorSubsystem>();
	}};

	/**
	 * Spawns a StaticMeshActor (or a derived type) at the specified transform.
	 */
	template <typename T = AStaticMeshActor>
	static T* SpawnStaticMesh(const FTransform& Transform)
	{
		if (UEditorActorSubsystem* Subsystem = ActorSubsystem.Get())
		{
			if (AActor* Actor = Subsystem->SpawnActorFromClass(T::StaticClass(),
				Transform.GetTranslation(), Transform.Rotator(), false))
			{
				Actor->SetActorScale3D(Transform.GetScale3D());
				return Cast<T>(Actor);
			}
		}

		return nullptr;
	}

	/**
	 * Safely destroys actors within an undoable editor transaction.
	 * @param Actors Actors to delete.
	 */
	static void DestroyActors(const TArray<AActor*>& Actors);

	/**
	 * Splits mesh components from the source actors into individual mesh actors.
	 * @param Sources Actors to search mesh components for splitting.
	 * @param bDestroySources If true, source actors will be deleted afterward.
	 * @return Actors spawned during the process.
	 */
	static TArray<AActor*> SplitActors(const TArray<AActor*>& Sources, const bool bDestroySources);

	/**
	 * Combines similar mesh components from the source actors into instanced mesh actors.
	 * @param Sources Actors to search mesh components for instancing.
	 * @param bUseHISM If true, Hierarchical ISMs will be used for LoD support.
	 * @param bDestroySources If true, source actors will be deleted afterward.
	 * @return Actors spawned during the process.
	 */
	static TArray<AActor*> InstanceActors(const TArray<AActor*>& Sources, const bool bUseHISM, const bool bDestroySources);

private:

	static TArray<AActor*> ConvertMeshComponent(const UMeshComponent* Src);

	static AActor* ConvertStaticMesh(const UStaticMeshComponent* Src);
	static AActor* ConvertSplineMesh(const USplineMeshComponent* Src);
	static AActor* ConvertSkeletalMesh(const USkeletalMeshComponent* Src);
	static AActor* ConvertProceduralMesh(const UProceduralMeshComponent* Src);

	static TMap<FStaticMeshProperties, TArray<FTransform>> GroupSimilarMeshes(TArray<UMeshComponent*>& Srcs);
#endif
};
