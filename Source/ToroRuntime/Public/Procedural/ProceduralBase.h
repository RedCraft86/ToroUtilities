// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "Helpers/ActorBaking.h"
#include "GameFramework/Actor.h"
#include "DataTypes/MeshProperties.h"
#include "ProceduralBase.generated.h"

/** 
 * Defines how indices are selected when iterating through a source array 
 * to generate procedural content.
 */
UENUM(BlueprintType)
enum class EProceduralLoopMode : uint8
{
	/** Standard sequence: 0, 1, 2, ... N */
	Forward,

	/** Reversed sequence: N, N-1, ... 0 */
	Reverse,

	/** Random selection from the available indices */
	Random
};

/**
 * The foundation class for editor-driven procedural actors.
 * Manages the construction lifecycle, component registration, and mesh baking.
 */
UCLASS(Abstract, PrioritizeCategories = (Settings, Tools), HideCategories = (HLOD, Replication, Collision, Physics, Networking, Input))
class TORORUNTIME_API AProceduralBase : public AActor
{
	GENERATED_BODY()

public:

	AProceduralBase();

	/**
	 * Generates a list of indices based on a loop mode.
	 * @param Sample - The source properties to reference.
	 * @param Mode   - The iteration strategy (Forward, Reverse, Random).
	 * @param Amount - Total number of indices to generate.
	 * @return An array of byte indices mapped to the Sample array.
	 */
	UFUNCTION(BlueprintCallable, Category = Procedural)
		static TArray<uint8> LoopMeshArray(const TArray<FStaticMeshProperties>& Sample, const EProceduralLoopMode Mode, const uint8 Amount);

	template<typename T>
	T* AddGenericComponent()
	{
		if (T* NewComp = NewObject<T>(this))
		{
			NewComp->CreationMethod = EComponentCreationMethod::UserConstructionScript;
			NewComp->OnComponentCreated();
			NewComp->RegisterComponent();

			NewComp->SetMobility(EComponentMobility::Movable);
			NewComp->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
			return NewComp;
		}
		return nullptr;
	}

protected:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<USceneComponent> SceneRoot;

	/** If true, the actor will run construction every time it is moved in the viewport or a property is changed. */
	UPROPERTY(EditAnywhere, Category = Settings, AdvancedDisplay)
		bool bRealtimeConstruction;

#if WITH_EDITORONLY_DATA
	/** If enabled, the original procedural actor is deleted after the Bake operation is finished. */
	UPROPERTY(EditAnywhere, Category = Tools, DisplayName = "Destroy After Bake")
		bool bBakeDestroySource = true;
#endif

	/** Editor-facing button to manually trigger the procedural construction logic. */
	UFUNCTION(CallInEditor, Category = Tools)
		virtual void Construct() { EventConstruct(); }

	/** Entry point for Blueprint-based procedural logic. */
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Construct")
		void EventConstruct();

#if WITH_EDITOR
	/** 
	 * Converts procedural geometry (ProcMesh/ISM) into static actors in the level.
	 * Utilizes FActorBaking to generate permanent assets.
	 */
	UFUNCTION(CallInEditor, Category = Tools, DisplayName = "Bake Meshes")
		virtual void BakeActor() { FActorBaking::BakeActors({this}, bBakeDestroySource); }
#endif

	virtual void OnConstruction(const FTransform& Transform) override;
};