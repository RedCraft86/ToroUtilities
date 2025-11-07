// Copyright (C) RedCraft86. All Rights Reserved.

#include "LightProbes/LightProbeActor.h"
#include "LightProbes/LightProbeManager.h"

ALightProbeActor::ALightProbeActor(): Intensity(1.0f), Radius(500.0f), Falloff(2.0f)
	, Color(FLinearColor::White), bDisableWithLumenGI(false), MaxDistance(3000.0f), FadeRange(100.0f)
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

#if WITH_EDITOR
	EditorShapes = CreateEditorOnlyDefaultSubobject<UEditorShapeComponent>("EditorShapes");
	IconBillboard = CreateEditorOnlyDefaultSubobject<UMaterialBillboardComponent>("IconBillboard");
	if (IconBillboard)
	{
		IconBillboard->SetupAttachment(GetRootComponent());
		IconBillboard->SetHiddenInGame(true);
	}
#endif
}

bool ALightProbeActor::IsRelevantProbe(const FTransform& Camera, const bool bHasLumenGI) const
{
#if WITH_EDITOR
	if (!(FApp::IsGame() ? IsEnabled() : GetEnabledState_Implementation())
#else
	if (!IsEnabled()
#endif
		|| Radius < 50.0f
		|| FMath::IsNearlyZero(Intensity)
		|| (bHasLumenGI && bDisableWithLumenGI))
		return false;

	const FVector ThisLocation = GetActorLocation();
	FVector CameraLocation = Camera.GetTranslation();
	if (FVector::Dist(ThisLocation, CameraLocation) > MaxDistance) return false;

	CameraLocation -= Camera.GetRotation().Vector() * Radius;
	FVector ProbeToCam = ThisLocation - CameraLocation; ProbeToCam.Normalize();
	return FVector::DotProduct(Camera.GetRotation().Vector(), ProbeToCam) > 0.0f;
}

void ALightProbeActor::ApplyData(UMaterialInstanceDynamic* Material, const uint8 Idx, const FVector& Camera) const
{
	if (Material)
	{
		float Multi = Intensity;
		if (FadeRange > 50.0f)
		{
			Multi *= FMath::GetMappedRangeValueClamped(
			   FVector2D{MaxDistance, MaxDistance - FadeRange},
			   FVector2D{0.0f, 1.0f},
			   FVector::Dist(GetActorLocation(), Camera));
		}

		Material->SetVectorParameterValue(ULightProbeManager::ProbeParam(Idx, false), FLinearColor(
			Color.R * Multi,
			Color.G * Multi,
			Color.B * Multi,
			Falloff
		));

		const FVector Pos = GetActorLocation();
		Material->SetVectorParameterValue(ULightProbeManager::ProbeParam(Idx, true), FLinearColor(
			(float)Pos.X, (float)Pos.Y, (float)Pos.Z, Radius
		));
	}
}

void ALightProbeActor::EnableStateChanged(const bool bState)
{
	SetActorHiddenInGame(!bState);
}

#if WITH_EDITOR
void ALightProbeActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (EditorShapes)
	{
		FWireSphereData Data;
		Data.NumOfSides = 64;
		Data.Radius = Radius;
		Data.Color = FColor::FromHex(TEXT("97C1D0FF"));
		EditorShapes->WireSpheres.Add(TEXT("Radius"), Data);
	}
	if (IconBillboard)
	{
		if (!IconMaterial)
		{
			if (UMaterialInterface* BaseMat = LoadObject<UMaterialInterface>(nullptr,
				TEXT("/ToroUtilities/Icons/LightProbe/M_LightProbe.M_LightProbe")))
			{
				IconMaterial = UMaterialInstanceDynamic::Create(BaseMat, IconBillboard.Get());
			}
			
			if (!IconBillboard->Elements.IsEmpty()) IconBillboard->SetElements({});
		}

		if (IconMaterial)
		{
			IconMaterial->SetVectorParameterValue(TEXT("Color"), Color);
			if (IconBillboard->Elements.IsEmpty())
			{
				FMaterialSpriteElement Sprite;
				Sprite.Material = IconMaterial.Get();
				Sprite.BaseSizeX = Sprite.BaseSizeY = 40.0f;
				Sprite.DistanceToSizeCurve = LoadObject<UCurveFloat>(nullptr,
					TEXT("/ToroUtilities/Icons/LightProbe/C_LightProbe.C_LightProbe"));
				
				IconBillboard->SetElements({Sprite});
			}
		}
	}
}
#endif