// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ComponentVisualizer.h"
#include "ComponentVis/VisualizerHelpers.h"
#include "Components/EditorShapeComponent.h"

class FEditorShapeVisualizer final : public FComponentVisualizer
{
public:
	
	static TSharedPtr<FComponentVisualizer> MakeInstance()
	{
		return MakeShareable(new FEditorShapeVisualizer);
	}
	
private:

	virtual void DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI) override
	{
		if (const UEditorShapeComponent* VizComponent = Cast<const UEditorShapeComponent>(Component))
		{
			for (const TPair<FName, FWirePointData>& Point : VizComponent->WirePoints)
			{
				PDI->DrawPoint(VizComponent->TransformLocation(Point.Value.Location), Point.Value.Color,
					Point.Value.Size, Point.Value.bRenderOnTopOfEverything ? SDPG_Foreground : SDPG_World);
			}

			for (const TPair<FName, FWireLineData>& Line : VizComponent->WireLines)
			{
				PDI->DrawLine(VizComponent->TransformLocation(Line.Value.Start),
					VizComponent->TransformLocation(Line.Value.End), Line.Value.Color,
					Line.Value.bRenderOnTopOfEverything ? SDPG_Foreground : SDPG_World, Line.Value.Thickness);
			}

			for (const TPair<FName, FWireCircleData>& Circle : VizComponent->WireCircles)
			{
				DrawCircle(PDI, VizComponent->TransformLocation(Circle.Value.Location),
					VizComponent->AsForwardVector(Circle.Value.Rotation),
					VizComponent->AsRightVector(Circle.Value.Rotation),
					Circle.Value.Color, Circle.Value.Radius, Circle.Value.NumOfSides,
					Circle.Value.bRenderOnTopOfEverything ? SDPG_Foreground : SDPG_World, Circle.Value.Thickness);
			}

			for (const TPair<FName, FWireArcData>& Arc : VizComponent->WireArcs)
			{
				VisualizerHelpers::DrawArcWithThickness(PDI, VizComponent->TransformLocation(Arc.Value.Location),
					VizComponent->AsForwardVector(Arc.Value.Rotation),
					VizComponent->AsRightVector(Arc.Value.Rotation),
					Arc.Value.MinAngle, Arc.Value.MaxAngle, Arc.Value.Radius, Arc.Value.Sections, Arc.Value.Color,
					Arc.Value.bRenderOnTopOfEverything ? SDPG_Foreground : SDPG_World, Arc.Value.Thickness);
			}

			for (const TPair<FName, FWireBoxData>& Box : VizComponent->WireBoxes)
			{
				DrawOrientedWireBox(PDI, VizComponent->TransformLocation(Box.Value.Location),
					VizComponent->AsForwardVector(Box.Value.Rotation),
					VizComponent->AsRightVector(Box.Value.Rotation),
					VizComponent->AsUpVector(Box.Value.Rotation),
					Box.Value.Extents, Box.Value.Color,
					Box.Value.bRenderOnTopOfEverything ? SDPG_Foreground : SDPG_World, Box.Value.Thickness);
			}

			for (const TPair<FName, FWireSphereData>& Sphere : VizComponent->WireSpheres)
			{
				DrawWireSphere(PDI, VizComponent->TransformLocation(Sphere.Value.Location),
					Sphere.Value.Color, Sphere.Value.Radius, Sphere.Value.NumOfSides,
					Sphere.Value.bRenderOnTopOfEverything ? SDPG_Foreground : SDPG_World, Sphere.Value.Thickness);
			}

			for (const TPair<FName, FWireCylinderData>& Cylinder : VizComponent->WireCylinders)
			{
				DrawWireCylinder(PDI, VizComponent->TransformLocation(Cylinder.Value.Location),
					VizComponent->AsForwardVector(Cylinder.Value.Rotation),
					VizComponent->AsRightVector(Cylinder.Value.Rotation),
					VizComponent->AsUpVector(Cylinder.Value.Rotation),
					Cylinder.Value.Color, Cylinder.Value.Radius, Cylinder.Value.HalfHeight, Cylinder.Value.NumOfSides,
					Cylinder.Value.bRenderOnTopOfEverything ? SDPG_Foreground : SDPG_World, Cylinder.Value.Thickness);
			}

			for (const TPair<FName, FWireCapsuleData>& Capsule : VizComponent->WireCapsules)
			{
				DrawWireCapsule(PDI, VizComponent->TransformLocation(Capsule.Value.Location),
					VizComponent->AsForwardVector(Capsule.Value.Rotation),
					VizComponent->AsRightVector(Capsule.Value.Rotation),
					VizComponent->AsUpVector(Capsule.Value.Rotation),
					Capsule.Value.Color, Capsule.Value.Radius, Capsule.Value.HalfHeight, Capsule.Value.NumOfSides,
					Capsule.Value.bRenderOnTopOfEverything ? SDPG_Foreground : SDPG_World, Capsule.Value.Thickness);
			}

			const int32 NumNavPoints = VizComponent->NavPathPoints.Num();
			for (int i = 0; i < NumNavPoints; i++)
			{
				PDI->DrawPoint(VizComponent->NavPathPoints[i], VizComponent->WireNavPath.PointColor,
					VizComponent->WireNavPath.PointSize, VizComponent->WireNavPath.bRenderOnTopOfEverything ? SDPG_Foreground : SDPG_World);

				if (i < NumNavPoints - 1)
				{
					const FVector Next = VizComponent->NavPathPoints[i + 1];
					if (const FVector This = VizComponent->NavPathPoints[i]; !This.Equals(Next))
					{
						PDI->DrawLine( This, Next, VizComponent->WireNavPath.Color,
					   VizComponent->WireNavPath.bRenderOnTopOfEverything ? SDPG_Foreground : SDPG_World,
					   VizComponent->WireNavPath.LineThickness);
					}
				}
			}
		}
	}

	virtual void DrawVisualizationHUD(const UActorComponent* Component, const FViewport* Viewport, const FSceneView* View, FCanvas* Canvas) override
	{
		if (const UEditorShapeComponent* VizComponent = Cast<const UEditorShapeComponent>(Component))
		{
			for (const TPair<FName, FWireStringData>& StringData : VizComponent->WireStrings)
			{
				VisualizerHelpers::DrawText(View, Canvas, VizComponent->TransformLocation(StringData.Value.Location),
					StringData.Value.String.IsEmpty() ? FText::FromName(StringData.Key) : FText::FromString(StringData.Value.String),
					StringData.Value.FontScale, StringData.Value.Color, StringData.Value.bDrawShadow);
			}
		}
	}
};
