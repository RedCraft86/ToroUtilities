// Copyright (C) RedCraft86. All Rights Reserved.

#include "Graph/ToroGraphConnectionPolicy.h"
#include "Nodes/EdToroGraphNode.h"
#include "Nodes/EdToroGraphEdge.h"

FVector2f FToroGraphConnectionPolicy::ComputeSplineTangent(const FVector2f& Start, const FVector2f& End) const
{
	const FVector2f Delta = End - Start;
	return Delta.GetSafeNormal();
}

void FToroGraphConnectionPolicy::DetermineWiringStyle(UEdGraphPin* OutputPin, UEdGraphPin* InputPin, FConnectionParams& Params)
{
	Params.AssociatedPin1 = OutputPin;
	Params.AssociatedPin2 = InputPin;
	Params.WireThickness = 1.5f;
	if (!HoveredPins.IsEmpty())
	{
		ApplyHoverDeemphasis(OutputPin, InputPin, Params.WireThickness, Params.WireColor);
	}
}

void FToroGraphConnectionPolicy::Draw(TMap<TSharedRef<SWidget>, FArrangedWidget>& InPinGeometries, FArrangedChildren& ArrangedNodes)
{
	NodeWidgetMap.Empty();
	for (int32 NodeIndex = 0; NodeIndex < ArrangedNodes.Num(); NodeIndex++)
	{
		NodeWidgetMap.Add(StaticCastSharedRef<SGraphNode>(ArrangedNodes[NodeIndex].Widget)->GetNodeObj(), NodeIndex);
	}

	FConnectionDrawingPolicy::Draw(InPinGeometries, ArrangedNodes);
}

void FToroGraphConnectionPolicy::DrawSplineWithArrow(const FGeometry& StartGeom, const FGeometry& EndGeom, const FConnectionParams& Params)
{
	const FVector2f StartCenter = FGeometryHelper::CenterOf(StartGeom);
	const FVector2f EndCenter = FGeometryHelper::CenterOf(EndGeom);
	const FVector2f SeedPoint = (StartCenter + EndCenter) * 0.5f;

	const FVector2f StartAnchorPoint = FGeometryHelper::FindClosestPointOnGeom(StartGeom, SeedPoint);
	const FVector2f EndAnchorPoint = FGeometryHelper::FindClosestPointOnGeom(EndGeom, SeedPoint);

	DrawSplineWithArrow(StartAnchorPoint, EndAnchorPoint, Params);
}

void FToroGraphConnectionPolicy::DrawSplineWithArrow(const FVector2f& StartPoint, const FVector2f& EndPoint, const FConnectionParams& Params)
{
	const FVector2f& P0 = Params.bUserFlag1 ? EndPoint : StartPoint;
	const FVector2f& P1 = Params.bUserFlag1 ? StartPoint : EndPoint;

	Internal_DrawLineWithArrow(P0, P1, Params);
}

void FToroGraphConnectionPolicy::DrawPreviewConnector(const FGeometry& PinGeometry,
	const FVector2f& StartPoint, const FVector2f& EndPoint, UEdGraphPin* Pin)
{
	FConnectionParams Params;
	DetermineWiringStyle(Pin, nullptr, Params);
	if (Pin->Direction == EGPD_Output)
	{
		DrawSplineWithArrow(FGeometryHelper::FindClosestPointOnGeom(PinGeometry, EndPoint), EndPoint, Params);
	}
	else
	{
		DrawSplineWithArrow(FGeometryHelper::FindClosestPointOnGeom(PinGeometry, StartPoint), StartPoint, Params);
	}
}

void FToroGraphConnectionPolicy::DetermineLinkGeometry(FArrangedChildren& ArrangedNodes,
	TSharedRef<SWidget>& OutputPinWidget, UEdGraphPin* OutputPin, UEdGraphPin* InputPin,
	FArrangedWidget*& StartWidgetGeometry, FArrangedWidget*& EndWidgetGeometry)
{
	if (UEdToroGraphEdge* EdgeNode = Cast<UEdToroGraphEdge>(InputPin->GetOwningNode()))
	{
		const UEdToroGraphNode* Start = EdgeNode->GetStartNode();
		const UEdToroGraphNode* End = EdgeNode->GetEndNode();
		if (Start && End)
		{
			const int32* StartNodeIndex = NodeWidgetMap.Find(Start);
			const int32* EndNodeIndex = NodeWidgetMap.Find(End);
			if (StartNodeIndex && EndNodeIndex)
			{
				StartWidgetGeometry = &ArrangedNodes[*StartNodeIndex];
				EndWidgetGeometry = &ArrangedNodes[*EndNodeIndex];
			}
		}
	}
	else
	{
		StartWidgetGeometry = PinGeometries->Find(OutputPinWidget);
		if (const TSharedPtr<SGraphPin>* TargetWidget = PinToPinWidgetMap.Find(InputPin))
		{
			const TSharedRef<SGraphPin> InputWidget = TargetWidget->ToSharedRef();
			EndWidgetGeometry = PinGeometries->Find(InputWidget);
		}
	}
}

void FToroGraphConnectionPolicy::Internal_DrawLineWithArrow(const FVector2f& StartAnchorPoint,
	const FVector2f& EndAnchorPoint, const FConnectionParams& Params)
{
	constexpr float LineSeparationAmount = 4.5f;

	const FVector2f DeltaPos = EndAnchorPoint - StartAnchorPoint;
	const FVector2f UnitDelta = DeltaPos.GetSafeNormal();
	const FVector2f Normal = FVector2f(DeltaPos.Y, -DeltaPos.X).GetSafeNormal();

	const FVector2f DirectionBias = Normal * LineSeparationAmount;
	const FVector2f LengthBias = ArrowRadius.X * UnitDelta;
	const FVector2f StartPoint = StartAnchorPoint + DirectionBias + LengthBias;
	const FVector2f EndPoint = EndAnchorPoint + DirectionBias - LengthBias;

	DrawConnection(WireLayerID, StartPoint, EndPoint, Params);

	const FVector2f ArrowDrawPos = EndPoint - ArrowRadius;
	const float AngleInRadians = FMath::Atan2(DeltaPos.Y, DeltaPos.X);

	FSlateDrawElement::MakeRotatedBox(DrawElementsList, ArrowLayerID,
		FPaintGeometry(ArrowDrawPos, ArrowImage->ImageSize * ZoomFactor, ZoomFactor),
		ArrowImage, ESlateDrawEffect::None, AngleInRadians, TOptional<FVector2f>(),
		FSlateDrawElement::RelativeToElement, Params.WireColor);
}
