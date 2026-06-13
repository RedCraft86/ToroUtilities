// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#if WITH_EDITOR
#include "Components/Editor/EdIconComponent.h"
#include "Components/Editor/EdShapeComponent.h"
#endif
#include "GameFramework/Actor.h"
#include "LightProbeActor.generated.h"

/**
 * An actor representing a single lighting influence point.
 * Collected by ULightProbeManager to contribute to the global lighting buffer.
 */
UCLASS(NotBlueprintable, BlueprintType, meta = (HideCategories = "*", ShowCategories = "Settings Rendering Actor"))
class TORORUNTIME_API ALightProbeActor final : public AActor
{
	GENERATED_BODY()

public:

	ALightProbeActor();

	/**
	 * Packs the probe's state into two linear colors for shader consumption.
	 * @param ColorParam Output: RGB = Final Color, A = Falloff value.
	 * @param PosParam Output: XYZ = World Position, A = Effective Radius.
	 * @param Camera The current view location for distance-based calculations.
	 */
	void GetProbeData(FLinearColor& ColorParam, FLinearColor& PosParam, const FVector& Camera) const;

	/**
	 * Determines if this probe should contribute to the scene based on distance and engine state.
	 * @param Camera The current camera transform.
	 * @param bHasLumenGI True if Lumen Global Illumination is currently active.
	 */
	bool IsRelevantProbe(const FTransform& Camera, const bool bHasLumenGI) const;

private:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects, meta = (AllowPrivateAccess = true))
		TObjectPtr<USceneComponent> SceneRoot;

	/** Lighting intensity multiplier. */
	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = 1.0f, UIMin = 10.0f, UIMax = 100.0f))
		float Intensity;

	/** Maximum radius of the light influence. */
	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = 50.0f, UIMax = 1000.0f))
		float Radius;

	/** Sharpness of the light falloff at the edge of the radius. */
	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = 0.0f, UIMax = 8.0f))
		float Falloff;

	/** Base color of the probe light. */
	UPROPERTY(EditAnywhere, Category = Settings, meta = (HideAlphaChannel))
		FLinearColor Color;

	/** If true, this probe will be disabled when Lumen GI is active to prevent over-brightening. */
	UPROPERTY(EditAnywhere, Category = Settings, AdvancedDisplay)
		bool bDisableWithLumenGI;

	/** Maximum distance from the camera before the probe is ignored. */
	UPROPERTY(EditAnywhere, Category = Settings, AdvancedDisplay, meta = (ClampMin = 1000.0f, ClampMax = 10000.0f))
		float MaxDistance;

	/** Distance range over which the probe fades out as it approaches MaxDistance. */
	UPROPERTY(EditAnywhere, Category = Settings, AdvancedDisplay, meta = (ClampMin = 0.0f, ClampMax = 1000.0f))
		float FadeRange;

#if WITH_EDITOR
	DECLARE_ICON_COMPONENT
	TObjectPtr<UEdShapeComponent> EdShapeComp;

	virtual void OnConstruction(const FTransform& Transform) override;
#endif
};