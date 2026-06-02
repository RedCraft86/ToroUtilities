// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#if WITH_EDITOR
#include "Components/Editor/EdIconComponent.h"
#include "Components/Editor/EdShapeComponent.h"
#endif
#include "GameFramework/Actor.h"
#include "LightProbeActor.generated.h"

UCLASS(NotBlueprintable, BlueprintType, PrioritizeCategories = (Settings), HideCategories = (HLOD, Replication, Collision, Physics, Networking, Input, WorldPartition, LevelInstance, Cooking))
class TORORUNTIME_API ALightProbeActor final : public AActor
{
	GENERATED_BODY()

public:

	ALightProbeActor();

	// ColorParam( RGB + Falloff ) / PosParam( XYZ + Radius )
	void GetProbeData(FLinearColor& ColorParam, FLinearColor& PosParam, const FVector& Camera) const;
	bool IsRelevantProbe(const FTransform& Camera, const bool bHasLumenGI) const;

private:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects, meta = (AllowPrivateAccess = true))
		TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = 1.0f, UIMin = 10.0f, UIMax = 100.0f))
		float Intensity;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = 50.0f, UIMax = 1000.0f))
		float Radius;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = 0.0f, UIMax = 8.0f))
		float Falloff;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (HideAlphaChannel))
		FLinearColor Color;

	UPROPERTY(EditAnywhere, Category = Settings, AdvancedDisplay)
		bool bDisableWithLumenGI;

	UPROPERTY(EditAnywhere, Category = Settings, AdvancedDisplay, meta = (ClampMin = 1000.0f, ClampMax = 10000.0f))
		float MaxDistance;

	UPROPERTY(EditAnywhere, Category = Settings, AdvancedDisplay, meta = (ClampMin = 0.0f, ClampMax = 1000.0f))
		float FadeRange;

#if WITH_EDITOR
	DECLARE_ICON_COMPONENT
	TObjectPtr<UEdShapeComponent> EdShapeComp;

	virtual void OnConstruction(const FTransform& Transform) override;
#endif
};