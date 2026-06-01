// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "ComponentVis/ToroComponentVisualizer.h"
#include "Components/Editor/EdShapeComponent.h"

class FEdShapeVisualizer final : public FToroComponentVisualizer
{
	virtual void DrawVisualization(const UActorComponent* Component,
		const FSceneView* View, FPrimitiveDrawInterface* PDI) override
	{
		const UEdShapeComponent* ShapeComp = Cast<const UEdShapeComponent>(Component);
		if (!ShapeComp) return;

		for (const TPair<FName, FWirePointData>& Point : ShapeComp->WirePoints)
		{
			PDI->DrawPoint(ShapeComp->TransformLocation(Point.Value.Location), Point.Value.Color,
				Point.Value.Size, Point.Value.bRenderOnTop ? SDPG_Foreground : SDPG_World);
		}

		for (const TPair<FName, FWireLineData>& Line : ShapeComp->WireLines)
		{
			PDI->DrawLine(ShapeComp->TransformLocation(Line.Value.Start),
				ShapeComp->TransformLocation(Line.Value.End), Line.Value.Color,
				Line.Value.bRenderOnTop ? SDPG_Foreground : SDPG_World, Line.Value.Thickness);
		}

		for (const TPair<FName, FWireCircleData>& Circle : ShapeComp->WireCircles)
		{
			DrawCircle(PDI, ShapeComp->TransformLocation(Circle.Value.Location),
				ShapeComp->AsForwardVector(Circle.Value.Rotation),
				ShapeComp->AsRightVector(Circle.Value.Rotation),
				Circle.Value.Color, Circle.Value.Radius, Circle.Value.NumOfSides,
				Circle.Value.bRenderOnTop ? SDPG_Foreground : SDPG_World, Circle.Value.Thickness);
		}

		for (const TPair<FName, FWireArcData>& Arc : ShapeComp->WireArcs)
		{
			ToroVisualizerHelpers::DrawArcWithThickness(PDI, ShapeComp->TransformLocation(Arc.Value.Location),
				ShapeComp->AsForwardVector(Arc.Value.Rotation), ShapeComp->AsRightVector(Arc.Value.Rotation),
				Arc.Value.MinAngle, Arc.Value.MaxAngle, Arc.Value.Radius, Arc.Value.Sections, Arc.Value.Color,
				Arc.Value.bRenderOnTop ? SDPG_Foreground : SDPG_World, Arc.Value.Thickness);
		}

		for (const TPair<FName, FWireBoxData>& Box : ShapeComp->WireBoxes)
		{
			DrawOrientedWireBox(PDI, ShapeComp->TransformLocation(Box.Value.Location),
				ShapeComp->AsForwardVector(Box.Value.Rotation),
				ShapeComp->AsRightVector(Box.Value.Rotation),
				ShapeComp->AsUpVector(Box.Value.Rotation), Box.Value.Extents, Box.Value.Color,
				Box.Value.bRenderOnTop ? SDPG_Foreground : SDPG_World, Box.Value.Thickness);
		}

		for (const TPair<FName, FWireSphereData>& Sphere : ShapeComp->WireSpheres)
		{
			DrawWireSphere(PDI, ShapeComp->TransformLocation(Sphere.Value.Location),
				Sphere.Value.Color, Sphere.Value.Radius, Sphere.Value.NumOfSides,
				Sphere.Value.bRenderOnTop ? SDPG_Foreground : SDPG_World, Sphere.Value.Thickness);
		}

		for (const TPair<FName, FWireCylinderData>& Cylinder : ShapeComp->WireCylinders)
		{
			DrawWireCylinder(PDI, ShapeComp->TransformLocation(Cylinder.Value.Location),
				ShapeComp->AsForwardVector(Cylinder.Value.Rotation),
				ShapeComp->AsRightVector(Cylinder.Value.Rotation),
				ShapeComp->AsUpVector(Cylinder.Value.Rotation), Cylinder.Value.Color,
				Cylinder.Value.Radius, Cylinder.Value.HalfHeight, Cylinder.Value.NumOfSides,
				Cylinder.Value.bRenderOnTop ? SDPG_Foreground : SDPG_World, Cylinder.Value.Thickness);
		}

		for (const TPair<FName, FWireCapsuleData>& Capsule : ShapeComp->WireCapsules)
		{
			DrawWireCapsule(PDI, ShapeComp->TransformLocation(Capsule.Value.Location),
				ShapeComp->AsForwardVector(Capsule.Value.Rotation),
				ShapeComp->AsRightVector(Capsule.Value.Rotation),
				ShapeComp->AsUpVector(Capsule.Value.Rotation), Capsule.Value.Color,
				Capsule.Value.Radius, Capsule.Value.HalfHeight, Capsule.Value.NumOfSides,
				Capsule.Value.bRenderOnTop ? SDPG_Foreground : SDPG_World, Capsule.Value.Thickness);
		}

		for (const TPair<FName, FWireNavPathData>& NavPath : ShapeComp->WireNavPaths)
		{
			const int32 NumPoints = NavPath.Value.PathPoints.Num();
			for (int32 i = 0; i < NumPoints - 1; i++)
			{
				PDI->DrawPoint(NavPath.Value.PathPoints[i], NavPath.Value.PointColor,
					NavPath.Value.PointSize, NavPath.Value.bRenderOnTop ? SDPG_Foreground : SDPG_World);

				const FVector Next = NavPath.Value.PathPoints[i + 1];
				if (const FVector This = NavPath.Value.PathPoints[i]; !This.Equals(Next))
				{
					PDI->DrawLine( This, Next, NavPath.Value.Color,
						NavPath.Value.bRenderOnTop ? SDPG_Foreground : SDPG_World,
						NavPath.Value.LineThickness);
				}
			}
		}
	}

	virtual void DrawVisualizationHUD(const UActorComponent* Component,
		const FViewport* Viewport, const FSceneView* View, FCanvas* Canvas) override
	{
		const UEdShapeComponent* ShapeComp = Cast<const UEdShapeComponent>(Component);
		if (!ShapeComp) return;

		for (const TPair<FName, FWireStringData>& String : ShapeComp->WireStrings)
		{
			ToroVisualizerHelpers::DrawText(View, Canvas, ShapeComp->TransformLocation(String.Value.Location),
				String.Value.String.IsEmpty() ? FText::FromName(String.Key) : FText::FromString(String.Value.String),
				String.Value.FontScale, String.Value.Color, String.Value.bDrawShadow);
		}

		for (const TPair<FName, FWireNavPathData>& NavPath : ShapeComp->WireNavPaths)
		{
			if (NavPath.Value.NearestPoint.IsSet())
			{
				ToroVisualizerHelpers::DrawText(View, Canvas, NavPath.Value.NearestPoint.GetValue(),
					FText::FromName(NavPath.Key), 16, NavPath.Value.Color, true);
			}
		}
	}
};