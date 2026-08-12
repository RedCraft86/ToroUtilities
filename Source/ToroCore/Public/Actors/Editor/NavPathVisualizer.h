// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#pragma once

#include "GameFramework/Actor.h"
#include "NavPathVisualizer.generated.h"

/**
 * Actor to show the navigation system route to reach specified targets.
 * @note This actor is editor-only and will be removed at game runtime.
 */
UCLASS(MinimalAPI, NotBlueprintable, NotBlueprintType, PrioritizeCategories = (Settings),
	HideCategories = (Rendering, HLOD, Replication, Collision, Physics, Networking, Input, Actor, WorldPartition, LevelInstance, Cooking))
class ANavPathVisualizer final : public AActor
{
	GENERATED_BODY()

public:

	ANavPathVisualizer();

private:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects, meta = (AllowPrivateAccess = true))
		TObjectPtr<USceneComponent> SceneRoot;

#if WITH_EDITORONLY_DATA
	UPROPERTY(VisibleDefaultsOnly, Category = Subobjects)
		TObjectPtr<class UWireShapeComponent> ShapeComponent;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (MakeEditWidget = true))
		TArray<FVector> PathTargets;
#endif

#if WITH_EDITOR
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
#endif
};
