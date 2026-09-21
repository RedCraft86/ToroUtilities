// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#include "Components/TintedBillboardComponent.h"
FTintedSpriteSceneProxy::FTintedSpriteSceneProxy(const UBillboardComponent* InComp, float InScale, FLinearColor InColor)
	: FPrimitiveSceneProxy(InComp)
	, Origin(InComp->GetComponentLocation())
	, ScreenSize(InComp->ScreenSize)
	, Texture(InComp->Sprite)
	, Scale(InComp->GetComponentTransform().GetMaximumAxisScale() * InScale * 0.25f)
	, U(InComp->U)
	, ComponentUL(InComp->Sprite ? InComp->UL : 0.0f)
	, V(InComp->V)
	, ComponentVL(InComp->Sprite ? InComp->VL : 0.0f)
	, OpacityMaskRefVal(InComp->OpacityMaskRefVal)
	, Color(InColor)
	, bIsScreenSizeScaled(InComp->bIsScreenSizeScaled)
	, bIsActorLocked(false)
#if WITH_EDITORONLY_DATA
	, EditorScale(InComp->EditorScale)
#endif
{
	bWillEverBeLit = false;

#if WITH_EDITOR
	if (GIsEditor)
	{
		SpriteCategoryIdx = GEngine->GetSpriteCategoryIndex(InComp->SpriteInfo.Category);
	}
#endif

#if WITH_EDITORONLY_DATA
	if (const AActor* Owner = InComp->GetOwner())
	{
		bIsActorLocked = InComp->bShowLockedLocation && Owner->IsLockLocation();
	}
#endif
}

void FTintedSpriteSceneProxy::OnTransformChanged(FRHICommandListBase& RHICmdList)
{
	Origin = GetLocalToWorld().GetOrigin();
}

FPrimitiveViewRelevance FTintedSpriteSceneProxy::GetViewRelevance(const FSceneView* View) const
{
	bool bVisible = View->Family->EngineShowFlags.BillboardSprites;

#if WITH_EDITOR
	if (GIsEditor && bVisible && SpriteCategoryIdx != INDEX_NONE
		&& SpriteCategoryIdx < View->SpriteCategoryVisibility.Num())
	{
		bVisible = View->SpriteCategoryVisibility[SpriteCategoryIdx];
	}
#endif

	FPrimitiveViewRelevance Result;
	Result.bDrawRelevance = IsShown(View) && bVisible;
	Result.bOpaque = true;
	Result.bDynamicRelevance = true;
	Result.bShadowRelevance = IsShadowCast(View);
	Result.bEditorPrimitiveRelevance = UseEditorCompositing(View);
	return Result;
}

FVector2D FTintedSpriteSceneProxy::GetSpriteSize(const FSceneView* View, float UL, float VL) const
{
	float ViewScale = Scale;
	if (bIsScreenSizeScaled && View->ViewMatrices.GetViewToClip().M[3][3] != 1.0f)
	{
		const float ZoomFactor = FMath::Min<float>(
			View->ViewMatrices.GetViewToClip().M[0][0],
			View->ViewMatrices.GetViewToClip().M[1][1]);

		if (ZoomFactor != 0.0f)
		{
			const float Radius = View->WorldToScreen(Origin).W * (ScreenSize / ZoomFactor);
			if (Radius < 1.0f)
			{
				ViewScale *= Radius;
			}
		}
	}

#if WITH_EDITORONLY_DATA
	ViewScale *= EditorScale;
#endif
	return FVector2D(ViewScale * UL, ViewScale * VL);
}

FLinearColor FTintedSpriteSceneProxy::GetSpriteColor(const FSceneView* View) const
{
	FLinearColor SpriteColor = FLinearColor::White;
	if (View->Family->EngineShowFlags.ActorColoration && !IsSelected())
	{
		SpriteColor = GetPrimitiveColor();
	}
	else if (!bIsActorLocked)
	{
#if WITH_EDITOR
		if (View->bHasSelectedComponents && !IsIndividuallySelected())
		{
			SpriteColor += GEngine->GetSubduedSelectionOutlineColor()
				* GEngine->SelectionHighlightIntensityBillboards * 5;
		}
		else
#endif
		if (IsSelected())
		{
			SpriteColor += GEngine->GetSelectedMaterialColor()
				* GEngine->SelectionHighlightIntensityBillboards * 5;
		}
		else if (IsHovered())
		{
			SpriteColor += GEngine->GetHoveredMaterialColor()
				* GEngine->SelectionHighlightIntensityBillboards * 5;
		}
	}
	else
	{
		SpriteColor = FLinearColor::Red;
	}

	SpriteColor.A = 1.0f;
	return SpriteColor * Color;
}

void FTintedSpriteSceneProxy::GetDynamicMeshElements(const TArray<const FSceneView*>& Views,
	const FSceneViewFamily& ViewFamily, uint32 VisibilityMap, FMeshElementCollector& Collector) const
{
	QUICK_SCOPE_CYCLE_COUNTER(STAT_SpriteSceneProxy_GetDynamicMeshElements);
	const FTexture* TextureResource = Texture ? Texture->GetResource() : nullptr;
	if (!TextureResource)
	{
		return;
	}

	const float UL = ComponentUL == 0.0f ? TextureResource->GetSizeX() : ComponentUL;
	const float VL = ComponentVL == 0.0f ? TextureResource->GetSizeY() : ComponentVL;
	for (int32 ViewIndex = 0; ViewIndex < Views.Num(); ++ViewIndex)
	{
		if (!(VisibilityMap & (1u << ViewIndex)))
		{
			continue;
		}

		const FSceneView* View = Views[ViewIndex];
		const FVector2D SpriteSize = GetSpriteSize(View, UL, VL);
		Collector.GetPDI(ViewIndex)->DrawSprite(Origin, SpriteSize.X, SpriteSize.Y, TextureResource,
			GetSpriteColor(View), GetDepthPriorityGroup(View), U, UL, V, VL, SE_BLEND_Masked, OpacityMaskRefVal);

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
		RenderBounds(Collector.GetPDI(ViewIndex), View->Family->EngineShowFlags, GetBounds(), IsSelected());
#endif
	}
}

UTintedBillboardComponent::UTintedBillboardComponent(): SpriteColor(FLinearColor::White)
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	bAllowAnyoneToDestroyMe = true;
}

void UTintedBillboardComponent::SetSpriteColor(const FLinearColor InNewColor)
{
	if (!SpriteColor.Equals(InNewColor))
	{
		SpriteColor = InNewColor;
		MarkRenderStateDirty();
	}
}

FPrimitiveSceneProxy* UTintedBillboardComponent::CreateSceneProxy()
{
	float SpriteScale = 1.0f;

#if WITH_EDITOR
	if (GetOwner())
	{
		SpriteScale = GetOwner()->SpriteScale;
	}
#endif

	return new FTintedSpriteSceneProxy(this, SpriteScale, SpriteColor);
}
