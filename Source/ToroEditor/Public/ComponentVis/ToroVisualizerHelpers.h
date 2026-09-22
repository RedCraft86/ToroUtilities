// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#pragma once

#include "SceneView.h"
#include "CanvasItem.h"
#include "CanvasTypes.h"
#include "PrimitiveDrawInterface.h"
#include "PrimitiveDrawingUtils.h"
#include "Engine/Engine.h"

/**
 * Drawing helpers shared by Toro component visualizers.
 */
namespace ToroVisualizerHelpers
{
	/**
	 * Draws an arc with radial lines connecting both ends to its base.
	 * @param PDI Primitive drawing interface receiving the lines.
	 * @param Base Center of the arc.
	 * @param X Local axis corresponding to zero degrees.
	 * @param Y Local axis corresponding to ninety degrees.
	 * @param MinAngle Starting angle in degrees.
	 * @param MaxAngle Ending angle in degrees.
	 * @param Radius Arc radius in world units.
	 * @param Sections Number of line segments used for the arc.
	 * @param Color Line color and opacity.
	 * @param DepthPriority Scene depth-priority group.
	 * @param Thickness Line thickness.
	 */
	inline void DrawArcWithThickness(FPrimitiveDrawInterface* PDI, const FVector& Base, const FVector& X, const FVector& Y,
		const float MinAngle, const float MaxAngle, const float Radius, const int Sections,
		const FLinearColor& Color, const uint8 DepthPriority, const float Thickness)
	{
		if (!PDI || Sections <= 0)
		{
			return;
		}

		constexpr float ToRadians = PI / 180.0f;

		const float AngleStep = (MaxAngle - MinAngle) / Sections;
		float Angle = MinAngle;

		FVector LastVert = Base + Radius * (FMath::Cos(Angle * ToRadians) * X + FMath::Sin(Angle * ToRadians) * Y);
		Angle += AngleStep;

		for (int32 i = 0; i < Sections; i++)
		{
			FVector ThisVert = Base + Radius * (FMath::Cos(Angle * ToRadians) * X + FMath::Sin(Angle * ToRadians) * Y);
			if (i == 0)
			{
				PDI->DrawTranslucentLine(Base, LastVert, Color, DepthPriority, Thickness);
			}
			if (i == Sections - 1)
			{
				PDI->DrawTranslucentLine(Base, ThisVert, Color, DepthPriority, Thickness);
			}

			PDI->DrawTranslucentLine(LastVert, ThisVert, Color, DepthPriority, Thickness);
			LastVert = ThisVert;
			Angle += AngleStep;
		}
	}

	/**
	 * Draws text at a projected world-space location.
	 * @param View Scene view used to project the location.
	 * @param Canvas Canvas receiving the text item.
	 * @param Location World-space location of the text.
	 * @param Text Text to display.
	 * @param FontScale Font size, or twelve when set to zero.
	 * @param Color Text color and opacity.
	 * @param bDrawShadow Whether to draw a black text shadow.
	 * @param Font Font to use, or the engine small font when null.
	 */
	inline void DrawText(const FSceneView* View, FCanvas* Canvas, const FVector& Location,
		const FText& Text, const uint8 FontScale = 12, const FLinearColor& Color = FLinearColor::White,
		const bool bDrawShadow = false, const UFont* Font = nullptr)
	{
		if (!View || !Canvas || !GEngine)
		{
			return;
		}

		const UFont* DrawFont = Font ? Font : GEngine->GetSmallFont();
		if (!DrawFont)
		{
			return;
		}

		FVector2D ScreenPos;
		if (FSceneView::ProjectWorldToScreen(
			Location, View->UnconstrainedViewRect, View->ViewMatrices.GetWorldToClip(), ScreenPos
		))
		{
			const float DPIScale = Canvas->GetDPIScale();
			if (!FMath::IsNearlyZero(DPIScale))
			{
				ScreenPos /= DPIScale;
			}

			FCanvasTextItem TextItem = FCanvasTextItem(ScreenPos, Text, FSlateFontInfo(
				DrawFont,
				FontScale > 0 ? static_cast<float>(FontScale) : 12.0f,
				NAME_None
			), Color);

			if (bDrawShadow)
			{
				TextItem.EnableShadow(FColor::Black);
			}
			else
			{
				TextItem.DisableShadow();
			}

			Canvas->DrawItem(TextItem);
		}
	}
}
