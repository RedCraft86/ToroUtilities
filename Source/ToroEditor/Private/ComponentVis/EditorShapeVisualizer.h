// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#pragma once

#include "ComponentVis/ToroComponentVisualizer.h"
#include "Components/Editor/WireShapeComponent.h"

class FEditorShapeVisualizer final : public FToroComponentVisualizer
{
	TWeakObjectPtr<const UWireShapeComponent> ShapeComponent;

	FVector TransformLocation(const FVector& Location) const
	{
		if (ShapeComponent.IsValid() && ShapeComponent->GetOwner())
		{
			return ShapeComponent->GetOwner()->GetActorTransform().TransformPositionNoScale(Location);
		}

		return Location;
	}

	FRotator TransformRotation(const FRotator& Rotation) const
	{
		if (ShapeComponent.IsValid() && ShapeComponent->GetOwner())
		{
			return ShapeComponent->GetOwner()->GetActorTransform().TransformRotation(Rotation.Quaternion()).Rotator();
		}

		return Rotation;
	}

	FVector AsForwardVector(const FRotator& LocalRotation) const
	{
		return TransformRotation(LocalRotation).Vector();
	}

	FVector AsRightVector(const FRotator& LocalRotation) const
	{
		return FRotationMatrix(TransformRotation(LocalRotation)).GetScaledAxis(EAxis::Y);
	}

	FVector AsUpVector(const FRotator& LocalRotation) const
	{
		return FRotationMatrix(TransformRotation(LocalRotation)).GetScaledAxis(EAxis::Z);
	}

	virtual void DrawVisualization(const UActorComponent* Component,
		const FSceneView* View, FPrimitiveDrawInterface* PDI) override
	{
		ShapeComponent = Cast<UWireShapeComponent>(Component);
		if (!ShapeComponent.IsValid())
		{
			return;
		}

		for (const auto& [Name, PointData] : ShapeComponent->WirePoints)
		{
			PDI->DrawPoint(
				TransformLocation(PointData.Location),
				PointData.Color, PointData.Size,
				PointData.bRenderOnTop ? SDPG_Foreground : SDPG_World
			);
		}

		for (const auto& [Name, LineData] : ShapeComponent->WireLines)
		{
			PDI->DrawLine(
				TransformLocation(LineData.Start),
				TransformLocation(LineData.End), LineData.Color,
				LineData.bRenderOnTop ? SDPG_Foreground : SDPG_World,
				LineData.Thickness
			);
		}

		for (const auto& [Name, CircleData] : ShapeComponent->WireCircles)
		{
			DrawCircle(PDI,
				TransformLocation(CircleData.Location),
				AsForwardVector(CircleData.Rotation),
				AsRightVector(CircleData.Rotation),
				CircleData.Color, CircleData.Radius, CircleData.NumOfSides,
				CircleData.bRenderOnTop ? SDPG_Foreground : SDPG_World,
				CircleData.Thickness
			);
		}

		for (const auto& [Name, ArcData] : ShapeComponent->WireArcs)
		{
			ToroVisualizerHelpers::DrawArcWithThickness(PDI,
				TransformLocation(ArcData.Location),
				AsForwardVector(ArcData.Rotation),
				AsRightVector(ArcData.Rotation),
				ArcData.MinAngle, ArcData.MaxAngle,
				ArcData.Radius, ArcData.Sections, ArcData.Color,
				ArcData.bRenderOnTop ? SDPG_Foreground : SDPG_World,
				ArcData.Thickness
			);
		}

		for (const auto& [Name, BoxData] : ShapeComponent->WireBoxes)
		{
			DrawOrientedWireBox(PDI,
				TransformLocation(BoxData.Location),
				AsForwardVector(BoxData.Rotation),
				AsRightVector(BoxData.Rotation),
				AsUpVector(BoxData.Rotation),
				BoxData.Extents, BoxData.Color,
				BoxData.bRenderOnTop ? SDPG_Foreground : SDPG_World,
				BoxData.Thickness
			);
		}

		for (const auto& [Name, SphereData] : ShapeComponent->WireSpheres)
		{
			DrawWireSphere(PDI,
				TransformLocation(SphereData.Location),
				SphereData.Color, SphereData.Radius, SphereData.NumOfSides,
				SphereData.bRenderOnTop ? SDPG_Foreground : SDPG_World,
				SphereData.Thickness
			);
		}

		for (const auto& [Name, CylinderData] : ShapeComponent->WireCylinders)
		{
			DrawWireCylinder(PDI,
				TransformLocation(CylinderData.Location),
				AsForwardVector(CylinderData.Rotation),
				AsRightVector(CylinderData.Rotation),
				AsUpVector(CylinderData.Rotation),
				CylinderData.Color, CylinderData.Radius,
				CylinderData.HalfHeight, CylinderData.NumOfSides,
				CylinderData.bRenderOnTop ? SDPG_Foreground : SDPG_World,
				CylinderData.Thickness
			);
		}

		for (const auto& [Name, CapsuleData] : ShapeComponent->WireCapsules)
		{
			DrawWireCapsule(PDI,
				TransformLocation(CapsuleData.Location),
				AsForwardVector(CapsuleData.Rotation),
				AsRightVector(CapsuleData.Rotation),
				AsUpVector(CapsuleData.Rotation),
				CapsuleData.Color, CapsuleData.Radius,
				CapsuleData.HalfHeight, CapsuleData.NumOfSides,
				CapsuleData.bRenderOnTop ? SDPG_Foreground : SDPG_World,
				CapsuleData.Thickness
			);
		}

		for (const auto &[Name, NavPathData] : ShapeComponent->WireNavPaths)
		{
			const int32 NumPoints = NavPathData.PathPoints.Num();
			if (NumPoints < 2)
			{
				continue;
			}

			for (int32 i = 0; i < NumPoints - 1; i++)
			{
				PDI->DrawPoint(
					NavPathData.PathPoints[i],
					NavPathData.PointColor, NavPathData.PointSize,
					NavPathData.bRenderOnTop ? SDPG_Foreground : SDPG_World
				);

				const FVector Current = NavPathData.PathPoints[i];
				const FVector Next = NavPathData.PathPoints[i + 1];
				if (!Current.Equals(Next, 1.0f))
				{
					PDI->DrawLine(
					   Current, Next, NavPathData.Color,
					   NavPathData.bRenderOnTop ? SDPG_Foreground : SDPG_World,
					   NavPathData.LineThickness
				   );
				}
			}
		}
	}

	virtual void DrawVisualizationHUD(const UActorComponent* Component,
		const FViewport* Viewport, const FSceneView* View, FCanvas* Canvas) override
	{
		ShapeComponent = Cast<UWireShapeComponent>(Component);
		if (!ShapeComponent.IsValid())
		{
			return;
		}

		for (const auto &[Name, StringData] : ShapeComponent->WireStrings)
		{
			ToroVisualizerHelpers::DrawText(
				View, Canvas, TransformLocation(StringData.Location),
				StringData.Text.IsEmpty() ? FText::FromName(Name) : FText::FromString(StringData.Text),
				StringData.FontScale, StringData.Color, StringData.bDrawShadow
			);
		}

		for (const auto &[Name, NavPathData] : ShapeComponent->WireNavPaths)
		{
			if (NavPathData.LabelPoint.IsSet())
			{
				ToroVisualizerHelpers::DrawText(View, Canvas,
					NavPathData.LabelPoint.GetValue() - FVector(0.0f, 0.0f, 15.0f),
					FText::FromName(Name), 14, NavPathData.Color, true
				);
			}
		}
	}
};