// Copyright (C) RedCraft86. All Rights Reserved.

#include "DataTypes/LightingData.h"

float FLightDrawDistance::GetMaxDrawDistance() const
{
	return bUseDrawDistance ? FMath::Max(0.0f, MaxDrawDistance) : 0.0f;
}

float FLightDrawDistance::GetMaxDistanceFadeRange() const
{
	return bUseDrawDistance ? FMath::Max(0.0f, MaxDistanceFadeRange) : 0.0f;
}

bool FLightDrawDistance::IsValidData() const
{
	return MaxDistanceFadeRange < MaxDrawDistance && MaxDistanceFadeRange + MaxDrawDistance > 0.0f;
}

FBaseLightProperties FBaseLightProperties::GetBase() const
{
	FBaseLightProperties Properties;
	Properties.IntensityUnits = IntensityUnits;
	Properties.Intensity = Intensity;
	Properties.Color = Color;
	Properties.AttenuationRadius = AttenuationRadius;
	Properties.bUseTemperature = bUseTemperature;
	Properties.Temperature = Temperature;
	Properties.IndirectIntensity = IndirectIntensity;
	Properties.VolumetricScatteringIntensity = VolumetricScatteringIntensity;
	Properties.bLightCastShadows = bLightCastShadows;
	Properties.bCastVolumetricShadows = bCastVolumetricShadows;
	Properties.SpecularScale = SpecularScale;
	Properties.DrawDistance = DrawDistance;

	return Properties;
}

void FBaseLightProperties::CopyFrom(const FBaseLightProperties& Properties)
{
	IntensityUnits = Properties.IntensityUnits;
	Intensity = Properties.Intensity;
	Color = Properties.Color;
	AttenuationRadius = Properties.AttenuationRadius;
	bUseTemperature = Properties.bUseTemperature;
	Temperature = Properties.Temperature;
	IndirectIntensity = Properties.IndirectIntensity;
	VolumetricScatteringIntensity = Properties.VolumetricScatteringIntensity;
	bLightCastShadows = Properties.bLightCastShadows;
	bCastVolumetricShadows = Properties.bCastVolumetricShadows;
	SpecularScale = Properties.SpecularScale;
	DrawDistance = Properties.DrawDistance;
}

void FBaseLightProperties::CopyBaseData(const FBaseLightProperties& Properties)
{
	IntensityUnits = Properties.IntensityUnits;
	Intensity = Properties.Intensity;
	Color = Properties.Color;
	bUseTemperature = Properties.bUseTemperature;
	Temperature = Properties.Temperature;
}
