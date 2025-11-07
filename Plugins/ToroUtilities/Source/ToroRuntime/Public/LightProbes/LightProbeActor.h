// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Actors/ToroActor.h"
#if WITH_EDITOR
#include "Components/EditorShapeComponent.h"
#include "Components/MaterialBillboardComponent.h"
#endif
#include "LightProbeActor.generated.h"

UCLASS(NotBlueprintable, HideCategories = (Rendering, HLOD, Collision, Physics))
class TORORUNTIME_API ALightProbeActor final : public AToroActor
{
	GENERATED_BODY()

public:

	ALightProbeActor();

	bool IsRelevantProbe(const FTransform& Camera, const bool bHasLumenGI) const;
	void ApplyData(UMaterialInstanceDynamic* Material, const uint8 Idx, const FVector& Camera) const;

private:

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = 0.0f))
		float Intensity;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = 50.0f))
		float Radius;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = 0.0f))
		float Falloff;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (HideAlphaChannel))
		FLinearColor Color;

	UPROPERTY(EditAnywhere, Category = Settings, AdvancedDisplay)
		bool bDisableWithLumenGI;

	UPROPERTY(EditAnywhere, Category = Settings, AdvancedDisplay, meta = (ClampMin = 1000.0f))
		float MaxDistance;

	UPROPERTY(EditAnywhere, Category = Settings, AdvancedDisplay, meta = (ClampMin = 0.0f))
		float FadeRange;

#if WITH_EDITORONLY_DATA
	UPROPERTY() TObjectPtr<UEditorShapeComponent> EditorShapes;
	UPROPERTY() TObjectPtr<UMaterialBillboardComponent> IconBillboard;
	UPROPERTY() TObjectPtr<UMaterialInstanceDynamic> IconMaterial;
#endif

	virtual void EnableStateChanged(const bool bState) override;
#if WITH_EDITOR
	virtual void OnConstruction(const FTransform& Transform) override;
#endif
};
