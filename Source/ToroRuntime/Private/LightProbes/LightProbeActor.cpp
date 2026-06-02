// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "LightProbes/LightProbeActor.h"

ALightProbeActor::ALightProbeActor()
	: Intensity(1.0f), Radius(500.0f), Falloff(2.0f), Color(FLinearColor::White)
	, bDisableWithLumenGI(true), MaxDistance(3000.0f), FadeRange(100.0f)
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

#if WITH_EDITOR
	DEFINE_ICON_COMPONENT
	UPDATE_ICON_SIZE(0.5f)
	UPDATE_ICON_IMAGE("/ToroUtilities/LightProbes/T_LightProbe.T_LightProbe")
	EdShapeComp = CreateEditorOnlyDefaultSubobject<UEdShapeComponent>(TEXT("EdShapeComp"));
#endif

	bEnableAutoLODGeneration = false; // Include Actor in HLOD option

	SetCanBeDamaged(false);
}

void ALightProbeActor::GetProbeData(FLinearColor& ColorParam, FLinearColor& PosParam, const FVector& Camera) const
{
	float Multi = Intensity;
	if (FadeRange > 50.0f)
	{
		Multi *= FMath::GetMappedRangeValueClamped(FVector2D(MaxDistance, MaxDistance - FadeRange),
		   FVector2D::UnitY(), FVector::Dist(GetActorLocation(), Camera));
	}

	ColorParam = Color * Multi;
	ColorParam.A = Falloff;

	PosParam = FLinearColor(GetActorLocation());
	PosParam.A = Radius;
}

bool ALightProbeActor::IsRelevantProbe(const FTransform& Camera, const bool bHasLumenGI) const
{
	if (IsHidden()
		|| Radius < 50.0f
		|| Intensity < 0.1f
		|| (bHasLumenGI && bDisableWithLumenGI))
		return false;

	const FVector ThisLocation = GetActorLocation();
	FVector CameraLocation = Camera.GetTranslation();

	// Distance check
	if (FVector::Dist(ThisLocation, CameraLocation) > MaxDistance)
	{
		return false;
	}

	// Back up the camera by the Radius of this to make sure we don't cull it just because we can't see the origin
	CameraLocation -= Camera.GetRotation().Vector() * Radius;

	// Check is in-front of camera
	FVector ProbeToCam = ThisLocation - CameraLocation; ProbeToCam.Normalize();
	return FVector::DotProduct(Camera.GetRotation().Vector(), ProbeToCam) > 0.0f;
}

#if WITH_EDITOR
void ALightProbeActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	Color.A = 1.0f;
	if (EdIconComp)
	{
		EdIconComp->SetSpriteColor(Color);
	}
	if (EdShapeComp)
	{
		FWireSphereData Data;
		Data.NumOfSides = 64;
		Data.Radius = Radius;
		Data.Thickness = 1 + (Radius / 2000.0f);
		Data.Color = FColor::FromHex(TEXT("CBFFFFFF"));
		EdShapeComp->WireSpheres.Add(TEXT("Radius"), Data);
	}
}
#endif