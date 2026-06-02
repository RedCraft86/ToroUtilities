// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#if WITH_EDITOR
#include "DataTypes/CachedGetter.h"
#include "ProceduralMeshComponent.h"
#include "Engine/SplineMeshActor.h"
#include "Animation/SkeletalMeshActor.h"
#include "Subsystems/EditorActorSubsystem.h"
#include "ScopedTransaction.h"
#include "Editor.h"
#endif

/**
 * A static utility class designed to convert dynamic, procedural, or complex component 
 * data into standalone Actors. 
 * 
 * This is primarily used for "Baking" procedural mesh generation results into 
 * standard StaticMeshActors, or converting temporary editor visuals into permanent 
 * level geometry.
 * 
 * All functions are Editor-Only.
 */
class TOROCORE_API FActorBaking final
{
#if WITH_EDITOR
public:

	/** 
	 * Cached accessor for the Editor Actor Subsystem.
	 * Uses TCachedGetter to avoid repeated subsystem lookups in GEditor.
	 */
	static inline TCachedGetter<UEditorActorSubsystem> ActorSubsystem{[]()
	{
		return GEditor->GetEditorSubsystem<UEditorActorSubsystem>();
	}};

	/** 
	 * Spawns a StaticMeshActor (or a derived type) at the specified transform.
	 * Automatically handles registration with the current editor level.
	 */
	template <typename T = AStaticMeshActor>
	static T* SpawnStaticMesh(const FTransform& Transform)
	{
		if (UEditorActorSubsystem* Subsystem = ActorSubsystem.Get())
		{
			AActor* Actor = Subsystem->SpawnActorFromClass(T::StaticClass(), FVector::ZeroVector);
			if (Actor) Actor->SetActorTransform(Transform, false, nullptr, ETeleportType::ResetPhysics);
			return Cast<T>(Actor);
		}
		return nullptr;
	}

	/** 
	 * Safely destroys a collection of actors within an editor transaction.
	 * This ensures the destruction can be undone/redone via the Editor Undo History.
	 */
	static void DestroyActors(const TArray<AActor*>& Actors)
	{
		if (UEditorActorSubsystem* Subsystem = ActorSubsystem.Get())
		{
			const FScopedTransaction Transaction(NSLOCTEXT("ToroCore", "DestroyActors", "Destroy Actors"));
			Subsystem->DestroyActors(Actors);
		}
	}

	/** 
	 * Converts a set of source actors into baked versions.
	 * @param Sources         The list of actors to bake.
	 * @param bDestroySources If true, the original actors are removed after successful baking.
	 * @return                A list of the newly created baked actors.
	 */
	static TArray<AActor*> BakeActors(const TArray<AActor*>& Sources, const bool bDestroySources);

	/** 
	 * Bakes a specific mesh component into a standalone actor(s).
	 * @param Source The component containing the mesh data.
	 * @param Index  Optional index for random label name indexing.
	 * @return       A list of the newly created baked actors.
	 */
	static TArray<AActor*> BakeComponent(const UMeshComponent* Source, const int32 Index = -1);

	/** 
	 * Converts procedural mesh data (Runtime/ProceduralMeshComponent) into a static mesh asset/actor.
	 */
	static AActor* BakeProceduralMesh(UProceduralMeshComponent* Source, const FName& Name = NAME_None);

	static AStaticMeshActor* BakeStaticMesh(const UStaticMeshComponent* Src);
	static ASplineMeshActor* BakeSplineMesh(const USplineMeshComponent* Src);
	static ASkeletalMeshActor* BakeSkeletalMesh(const USkeletalMeshComponent* Src);
#endif
};
