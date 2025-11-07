// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroSplineGenBase.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "SplineBarrier.generated.h"

UCLASS(NotBlueprintable)
class TOROCORE_API ASplineBarrier final : public AToroSplineGenBase
{
	GENERATED_BODY()

public:

	ASplineBarrier();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<UInstancedStaticMeshComponent> WallMeshComponent;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = 0.5f, UIMin = 0.5f))
		float WallHeight;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = 0, UIMin = 0))
		TSet<int32> SkipIndexes;

	UPROPERTY(EditAnywhere, Category = Settings)
		FPrimitiveCollision Collision;

	UPROPERTY(EditAnywhere, Category = Settings, AdvancedDisplay)
		TObjectPtr<UStaticMesh> Mesh;

	UPROPERTY(EditAnywhere, Category = Settings, AdvancedDisplay)
		TObjectPtr<UMaterialInterface> Material;

private:
#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = Settings, AdvancedDisplay)
		TObjectPtr<UMaterialInterface> OverlayMaterial = nullptr;
#endif
#if WITH_EDITOR
	void LoadObjects();
	virtual bool CanEditChange(const FProperty* InProperty) const override;
#endif

	virtual void BeginPlay() override;
	virtual void Construct() override;
};
