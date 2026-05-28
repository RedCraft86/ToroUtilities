// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#if WITH_EDITOR
#include "Components/Editor/EdShapeComponent.h"
#endif
#include "Actors/ToroActor.h"
#include "NavPathVisualizer.generated.h"

/**
 * An editor-only utility actor used to visualize navigation paths between multiple points.
 * 
 * This actor uses the 'MakeEditWidget' metadata on the Targets array, allowing designers 
 * to move waypoints directly in the 3D viewport using the transform gizmo. The actor 
 * then requests navigation path data and renders it via the EdShapes component.
 */
UCLASS(MinimalAPI, NotBlueprintable, NotBlueprintType, PrioritizeCategories = (Settings))
class ANavPathVisualizer final : public AToroActor
{
	GENERATED_BODY()

public:

	ANavPathVisualizer();

private:

#if WITH_EDITORONLY_DATA
	UPROPERTY(VisibleDefaultsOnly, Category = Subobjects)
		TObjectPtr<UEdShapeComponent> EdShapes;

	/** The identifier used to track this specific path in the EdShapes map. */
	UPROPERTY(EditAnywhere, Category = Settings)
		FName Name = TEXT("Default");

	/** If true, the debug lines will be rendered on top of all other geometry (X-Ray). */
	UPROPERTY(EditAnywhere, Category = Settings)
		bool bDrawOnTop = true;

	/** 
	 * A list of waypoints relative to the Actor's location.
	 * 'MakeEditWidget' enables interactive 3D handles for each vector in the viewport.
	 */
	UPROPERTY(EditAnywhere, Category = Settings, meta = (MakeEditWidget = true))
		TArray<FVector> Targets;

	UPROPERTY() FName LastName = TEXT("Default");
#endif

#if WITH_EDITOR
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
#endif
};
