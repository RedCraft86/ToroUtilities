// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#pragma once

#include "SceneView.h"
#include "CanvasItem.h"
#include "CanvasTypes.h"
#include "PrimitiveDrawInterface.h"
#include "PrimitiveDrawingUtils.h"
#include "Engine/Engine.h"

namespace ToroVisualizerHelpers
{
	inline void DrawArcWithThickness(FPrimitiveDrawInterface* PDI, const FVector& Base, const FVector& X,
		const FVector& Y, const float MinAngle, const float MaxAngle, const float Radius, const int Sections,
		const FLinearColor& Color, const uint8 DepthPriority, const float Thickness)
	{
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
			else if (i == Sections - 1)
			{
				PDI->DrawTranslucentLine(Base, ThisVert, Color, DepthPriority, Thickness);
			}

			PDI->DrawTranslucentLine(LastVert, ThisVert, Color, DepthPriority, Thickness);
			LastVert = ThisVert;
			Angle += AngleStep;
		}
	}

	inline void DrawText(const FSceneView* View, FCanvas* Canvas, const FVector& Location,
		const FText& Text, const uint8 FontScale = 12, const FLinearColor& Color = FLinearColor::White,
		const bool bDrawShadow = false, const UFont* Font = nullptr)
	{
		FVector2D ScreenPos;
		if (FSceneView::ProjectWorldToScreen(
			Location, View->UnconstrainedViewRect, View->ViewMatrices.GetWorldToClip(), ScreenPos
		))
		{
			ScreenPos /= Canvas->GetDPIScale();
			FCanvasTextItem TextItem = FCanvasTextItem(ScreenPos, Text, FSlateFontInfo(
				Font ? Font : GEngine->GetSmallFont(), 
				FontScale > 0 ? static_cast<float>(FontScale) : 12.0f, 
				NAME_None
			), Color);

			bDrawShadow ? TextItem.EnableShadow(FColor::Black) : TextItem.DisableShadow();
			Canvas->DrawItem(TextItem);
		}
	}
}