// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#include "Components/TintedBillboardComponent.h"

FTintedSpriteSceneProxy::FTintedSpriteSceneProxy(const UBillboardComponent* InComp, float InScale, FLinearColor InColor)
	: FPrimitiveSceneProxy(InComp), ScreenSize(InComp->ScreenSize), U(InComp->U), V(InComp->V)
	, Color(InColor), bIsScreenSizeScaled(InComp->bIsScreenSizeScaled), bIsActorLocked(false)
{
	bWillEverBeLit = false;
	Scale = InComp->GetComponentTransform().GetMaximumAxisScale() * InScale * 0.25f;
	OpacityMaskRefVal = InComp->OpacityMaskRefVal;
	if (InComp->Sprite)
	{
		Texture = InComp->Sprite;
		ComponentUL = InComp->UL;
		ComponentVL = InComp->VL;
	}
	else
	{
		Texture = nullptr;
		ComponentUL = ComponentVL = 0;
	}

#if WITH_EDITOR
	if (GIsEditor)
	{
		SpriteCategoryIdx = GEngine->GetSpriteCategoryIndex(InComp->SpriteInfo.Category);
	}
#endif
#if WITH_EDITORONLY_DATA
	EditorScale = InComp->EditorScale;
	bUseInEditorScaling = InComp->bUseInEditorScaling;
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
	if (GIsEditor 
		&& bVisible && SpriteCategoryIdx != INDEX_NONE 
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

void FTintedSpriteSceneProxy::GetDynamicMeshElements(const TArray<const FSceneView*>& Views,
	const FSceneViewFamily& ViewFamily, uint32 VisibilityMap, FMeshElementCollector& Collector) const
{
	QUICK_SCOPE_CYCLE_COUNTER(STAT_SpriteSceneProxy_GetDynamicMeshElements);
	if (const FTexture* TextureResource = Texture ? Texture->GetResource() : nullptr)
	{
		const float UL = ComponentUL == 0.0f ? TextureResource->GetSizeX() : ComponentUL;
		const float VL = ComponentVL == 0.0f ? TextureResource->GetSizeY() : ComponentVL;
		for (int32 ViewIndex = 0; ViewIndex < Views.Num(); ViewIndex++)
		{
			if (VisibilityMap & (1 << ViewIndex))
			{
				const FSceneView* View = Views[ViewIndex];
				float ViewedSizeX = Scale * UL;
				float ViewedSizeY = Scale * VL;

				if (bIsScreenSizeScaled && View->ViewMatrices.GetViewToClip().M[3][3] != 1.0f)
				{
					const float ZoomFactor = FMath::Min<float>(
						View->ViewMatrices.GetViewToClip().M[0][0], 
						View->ViewMatrices.GetViewToClip().M[1][1]
					);

					if (ZoomFactor != 0.0f)
					{
						const float Radius = View->WorldToScreen(Origin).W * (ScreenSize / ZoomFactor);

						if (Radius < 1.0f)
						{
							ViewedSizeX *= Radius;
							ViewedSizeY *= Radius;
						}
					}
				}

#if WITH_EDITORONLY_DATA
				ViewedSizeX *= EditorScale;
				ViewedSizeY *= EditorScale;
#endif

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
				Collector.GetPDI(ViewIndex)->DrawSprite(Origin, ViewedSizeX, ViewedSizeY, TextureResource, 
					SpriteColor * Color, GetDepthPriorityGroup(View), U, UL, V, VL, SE_BLEND_Masked, OpacityMaskRefVal
				);
			}
		}

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
		for (int32 ViewIndex = 0; ViewIndex < Views.Num(); ViewIndex++)
		{
			if (VisibilityMap & (1 << ViewIndex))
			{
				const FSceneView* View = Views[ViewIndex];
				RenderBounds(Collector.GetPDI(ViewIndex), View->Family->EngineShowFlags, GetBounds(), IsSelected());
			}
		}
#endif
	}
}

UTintedBillboardComponent::UTintedBillboardComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	bAllowAnyoneToDestroyMe = true;

	SpriteColor = FLinearColor::White;
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
