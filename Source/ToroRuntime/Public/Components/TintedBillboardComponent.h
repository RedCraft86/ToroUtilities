// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#pragma once

#include "PrimitiveSceneProxy.h"
#include "Components/BillboardComponent.h"
#include "TintedBillboardComponent.generated.h"

/**
 * Renders a billboard sprite with an additional color tint.
 * Based on FSpriteSceneProxy in Engine/Private/Components/BillboardComponent.cpp.
 */
class FTintedSpriteSceneProxy final : public FPrimitiveSceneProxy
{
public:

	FTintedSpriteSceneProxy(const UBillboardComponent* InComp, float InScale, FLinearColor InColor);

	virtual void OnTransformChanged(FRHICommandListBase& RHICmdList) override;
	virtual FPrimitiveViewRelevance GetViewRelevance(const FSceneView* View) const override;
	virtual void GetDynamicMeshElements(const TArray<const FSceneView*>& Views, const FSceneViewFamily& ViewFamily,
		uint32 VisibilityMap, FMeshElementCollector& Collector) const override;

	virtual uint32 GetMemoryFootprint(void) const override
	{
		return sizeof(*this) + GetAllocatedSize();
	}

	virtual SIZE_T GetTypeHash() const override
	{
		static size_t UniquePointer;
		return reinterpret_cast<size_t>(&UniquePointer);
	}

private:

	FVector2D GetSpriteSize(const FSceneView* View, float UL, float VL) const;
	FLinearColor GetSpriteColor(const FSceneView* View) const;

	FVector Origin;
	const float ScreenSize;
	const UTexture2D* Texture;
	const float Scale;
	const float U;
	const float ComponentUL;
	const float V;
	const float ComponentVL;
	const float OpacityMaskRefVal;
	const FLinearColor Color;
	const uint32 bIsScreenSizeScaled: 1;
	uint32 bIsActorLocked: 1;
#if WITH_EDITORONLY_DATA
	int32 SpriteCategoryIdx = INDEX_NONE;
	float EditorScale;
#endif
};

/**
 * Displays a billboard sprite with a configurable color tint.
 */
UCLASS(ClassGroup = (Rendering), meta = (BlueprintSpawnableComponent))
class TORORUNTIME_API UTintedBillboardComponent : public UBillboardComponent
{
	GENERATED_BODY()

public:

	UTintedBillboardComponent();

	/**
	 * Changes the sprite tint and refreshes its render state.
	 * @param InNewColor Tint to apply to the sprite.
	 */
	UFUNCTION(BlueprintCallable, Category = "Rendering|Components|Sprite")
		virtual void SetSpriteColor(const FLinearColor InNewColor);

	/**
	 * Gets the sprite tint.
	 */
	UFUNCTION(BlueprintPure, Category = "Rendering|Components|Sprite")
		const FLinearColor& GetSpriteColor() const { return SpriteColor; }

protected:

	/**
	 * Color multiplied with the sprite's rendered color.
	 */
	UPROPERTY(EditAnywhere, Category = Sprite)
		FLinearColor SpriteColor;

	virtual FPrimitiveSceneProxy* CreateSceneProxy() override;
};
