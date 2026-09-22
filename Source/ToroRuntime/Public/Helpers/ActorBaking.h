// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#pragma once

#if WITH_EDITOR
#include "DataTypes/CachedGetter.h"
#include "DataTypes/MeshSettings.h"
#include "ProceduralMeshComponent.h"
#include "Subsystems/EditorActorSubsystem.h"
#endif

/**
 * Provides editor-only helpers for converting and grouping mesh actors.
 */
class TORORUNTIME_API FActorBaking final
{
#if WITH_EDITOR
public:

	/**
	 * Cached access to the editor actor subsystem used by baking operations.
	 */
	static inline TCachedGetter<UEditorActorSubsystem> ActorSubsystem{[] {
		return GEditor ? GEditor->GetEditorSubsystem<UEditorActorSubsystem>() : nullptr;
	}};


	/**
	 * Spawns a static mesh actor using a complete world transform.
	 * @tparam T Static mesh actor class to spawn.
	 * @param Transform World transform assigned to the spawned actor.
	 * @return The spawned actor, or nullptr when the editor subsystem cannot spawn it.
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
	 * Deletes a collection of actors through the editor actor subsystem.
	 * @param Actors Actors to delete.
	 */
	static void DestroyActors(const TArray<AActor*>& Actors);

	/**
	 * Converts each supported mesh component into a separate actor.
	 * @param Sources Actors whose mesh components should be converted.
	 * @param bDestroySources Whether to delete the source actors after conversion.
	 * @return Successfully created actors.
	 */
	static TArray<AActor*> SplitActors(const TArray<AActor*>& Sources, const bool bDestroySources);

	/**
	 * Groups compatible static meshes into instanced actors and splits unsupported meshes.
	 * @param Sources Actors whose mesh components should be processed.
	 * @param bUseHISM Whether grouped meshes use hierarchical instancing.
	 * @param bDestroySources Whether to delete the source actors after conversion.
	 * @return Successfully created actors.
	 */
	static TArray<AActor*> InstanceActors(const TArray<AActor*>& Sources, const bool bUseHISM, const bool bDestroySources);

	/**
	 * Lays out actors in the scene using the largest actor to determine the grid size.
	 * @param Targets Actors that will be laid out on the map.
	 * @param MaxColumns Max number of columns to have.
	 * @param YawValue Yaw value of the rotation to use.
	 * @param Offset Amount added on top of the grid size.
	 * @return Whether the process happened.
	 */
	static bool LayoutActors(const TArray<AActor*>& Targets, const uint8 MaxColumns, const float YawValue, const FVector2D& Offset);

private:

	static TArray<AActor*> ConvertMeshComponent(const UMeshComponent* Src);

	static AActor* ConvertStaticMesh(const UStaticMeshComponent* Src);
	static AActor* ConvertSplineMesh(const USplineMeshComponent* Src);
	static AActor* ConvertSkeletalMesh(const USkeletalMeshComponent* Src);
	static AActor* ConvertProceduralMesh(const UProceduralMeshComponent* Src);

	static void GroupSimilarMeshes(TArray<UMeshComponent*>& Src, TMap<FStaticMeshSettings, TArray<FTransform>>& Result);
#endif
};