// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "ProcSplineBase.h"
#include "DataTypes/PrimitiveCollision.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "SplineBarrier.generated.h"

/**
 * A simple implementation that places Instanced Static Meshes along a spline to create a wall.
 */
UCLASS(NotBlueprintable, NotBlueprintType)
class TORORUNTIME_API ASplineBarrier final : public AProcSplineBase
{
	GENERATED_BODY()

public:

	ASplineBarrier();

private:

	UPROPERTY(VisibleDefaultsOnly, Category = Subobjects)
		TObjectPtr<UInstancedStaticMeshComponent> WallMeshes;

	/** The vertical height of the generated barrier. */
	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = 0.5f, UIMin = 0.5f))
		float WallHeight;

	/** Indices of spline segments where no mesh should be spawned (e.g., for doors or gaps). */
	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = 0, UIMin = 0))
		TSet<int32> SkipSegments;

	/** Collision configuration applied to the generated meshes. */
	UPROPERTY(EditAnywhere, Category = Settings)
		FPrimitiveCollision Collision;

	UPROPERTY(EditAnywhere, Category = Settings, AdvancedDisplay)
		TObjectPtr<UStaticMesh> WallMesh;

	UPROPERTY(EditAnywhere, Category = Settings, AdvancedDisplay)
		TObjectPtr<UMaterialInterface> OverlayMaterial;

#if WITH_EDITOR
	virtual void Construct() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual bool CanEditChange(const FProperty* InProperty) const override;
#endif
};