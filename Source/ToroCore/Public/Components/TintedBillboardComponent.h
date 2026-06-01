// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "PrimitiveSceneProxy.h"
#include "Components/BillboardComponent.h"
#include "TintedBillboardComponent.generated.h"

/**
 * A specialized Scene Proxy for rendering billboard sprites with a custom color tint.
 * This object lives on the Render Thread and handles the drawing logic for the sprite.
 * 
 * Mirrors FSpriteSceneProxy in Engine/Source/Runtime/Engine/Private/Components/BillboardComponent.cpp
 */
class FTintedSpriteSceneProxy final : public FPrimitiveSceneProxy
{
public:

	FTintedSpriteSceneProxy(const UBillboardComponent* InComponent, float SpriteScale, FLinearColor SpriteColor);

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
	
	FVector Origin;
	const float ScreenSize;
	const UTexture2D* Texture;
	float Scale;
	const float U;
	float ComponentUL;
	const float V;
	float ComponentVL;
	float OpacityMaskRefVal;
	FLinearColor Color;
	const uint32 bIsScreenSizeScaled: 1;
	uint32 bIsActorLocked: 1;
#if WITH_EDITORONLY_DATA
	int32 SpriteCategoryIdx;
	bool bUseInEditorScaling;
	float EditorScale;
#endif
};

/**
 * A specialized version of the standard Billboard Component that allows for runtime color tinting.
 * Perfect for color-coded actor markers or dynamic UI-in-world icons.
 */
UCLASS(ClassGroup = (Rendering), meta = (BlueprintSpawnableComponent))
class TOROCORE_API UTintedBillboardComponent : public UBillboardComponent
{
	GENERATED_BODY()

public:

	UTintedBillboardComponent();

	/** 
	 * Updates the tint color of the sprite and marks the render state as dirty.
	 * @param InNewColor The new tint (Alpha is ignored if the material/proxy doesn't support it).
	 */
	UFUNCTION(BlueprintCallable, Category = "Rendering|Components|Sprite")
		virtual void SetSpriteColor(const FLinearColor InNewColor);

	/** @return The current tint color of the sprite. */
	UFUNCTION(BlueprintPure, Category = "Rendering|Components|Sprite")
		const FLinearColor& GetSpriteColor() const { return SpriteColor; }

protected:

	/** The color applied to the billboard texture. */
	UPROPERTY(EditAnywhere, Category = Sprite)
		FLinearColor SpriteColor;

	virtual FPrimitiveSceneProxy* CreateSceneProxy() override;
};
