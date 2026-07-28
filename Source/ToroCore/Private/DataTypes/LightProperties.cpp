// Copyright (C) 2026 Tayzar Linn. Licensed under LGPL-3.0, see project LICENSE file.

#include "DataTypes/LightProperties.h"

void FLightDrawDistance::FromLightComponent(const ULightComponent* Target)
{
	if (Target)
	{
		MaxDistance	= Target->MaxDrawDistance;
		FadeRange	= Target->MaxDistanceFadeRange;
		bUseDrawDistance = MaxDistance > UE_SMALL_NUMBER || FadeRange > UE_SMALL_NUMBER;
	}
}

void FLightDrawDistance::ToLightComponent(ULightComponent* Target) const
{
	if (Target)
	{
		Target->SetMaxDrawDistance(GetMaxDistance());
		Target->SetMaxDistanceFadeRange(GetFadeRange());
	}
}

void FBaseLightProperties::CopyFrom(const FBaseLightProperties& Properties)
{
	IntensityUnits		= Properties.IntensityUnits;
	Intensity			= Properties.Intensity;
	Color				= Properties.Color;
	AttenuationRadius	= Properties.AttenuationRadius;
	bUseTemperature		= Properties.bUseTemperature;
	Temperature			= Properties.Temperature;
	IndirectIntensity	= Properties.IndirectIntensity;
	VolumetricIntensity	= Properties.VolumetricIntensity;
	bLightCastShadows	= Properties.bLightCastShadows;
	bVolumetricShadows	= Properties.bVolumetricShadows;
	SpecularScale		= Properties.SpecularScale;
	bAllowMegaLights	= Properties.bAllowMegaLights;
	LightChannels		= Properties.LightChannels;
	DrawDistance		= Properties.DrawDistance;
}

void FBaseLightProperties::FromLightComponent(const ULightComponent* Target)
{
	if (Target)
	{
		Intensity			= Target->Intensity;
		Color				= Target->GetLightColor();
		bUseTemperature		= Target->bUseTemperature;
		Temperature			= Target->Temperature;
		IndirectIntensity	= Target->IndirectLightingIntensity;
		VolumetricIntensity	= Target->VolumetricScatteringIntensity;
		bLightCastShadows	= Target->CastShadows;
		bVolumetricShadows	= Target->bCastVolumetricShadow;
		SpecularScale		= Target->SpecularScale;
		bAllowMegaLights	= Target->bAllowMegaLights;
		LightChannels		= Target->LightingChannels;

		DrawDistance.FromLightComponent(Target);

		if (const ULocalLightComponent* LocalLight = Cast<ULocalLightComponent>(Target))
		{
			IntensityUnits		= LocalLight->IntensityUnits;
			AttenuationRadius	= LocalLight->AttenuationRadius;
		}
	}
}

void FBaseLightProperties::ToLightComponent(ULightComponent* Target) const
{
	if (Target)
	{
		Target->SetIntensity(Intensity);
		Target->SetLightColor(Color);
		Target->SetTemperature(Temperature); // Before SetUseTemperature to avoid overriding
		Target->SetUseTemperature(bUseTemperature);
		Target->SetIndirectLightingIntensity(IndirectIntensity);
		Target->SetVolumetricScatteringIntensity(VolumetricIntensity);
		Target->SetCastShadows(bLightCastShadows);
		Target->SetCastVolumetricShadow(bVolumetricShadows);
		Target->SetSpecularScale(SpecularScale);

		Target->bAllowMegaLights = bAllowMegaLights;
		Target->SetLightingChannels(LightChannels.bChannel0, LightChannels.bChannel1, LightChannels.bChannel2);

		DrawDistance.ToLightComponent(Target);

		if (ULocalLightComponent* LocalLight = Cast<ULocalLightComponent>(Target))
		{
			LocalLight->SetIntensityUnits(IntensityUnits);
			LocalLight->SetAttenuationRadius(AttenuationRadius);
		}
	}
}

void FPointLightProperties::FromLightComponent(const ULightComponent* Target)
{
	FBaseLightProperties::FromLightComponent(Target);
	if (const UPointLightComponent* PointLight = Cast<UPointLightComponent>(Target))
	{
		SourceRadius			= PointLight->SourceRadius;
		SoftSourceRadius		= PointLight->SoftSourceRadius;
		SourceLength			= PointLight->SourceLength;
		bInverseSquaredFalloff	= PointLight->bUseInverseSquaredFalloff;
		LightFalloffExponent	= PointLight->LightFalloffExponent;
	}
}

void FPointLightProperties::ToLightComponent(ULightComponent* Target) const
{
	FBaseLightProperties::ToLightComponent(Target);
	if (UPointLightComponent* PointLight = Cast<UPointLightComponent>(Target))
	{
		PointLight->SetSourceRadius(SourceRadius);
		PointLight->SetSoftSourceRadius(SoftSourceRadius);
		PointLight->SetSourceLength(SourceLength);
		PointLight->SetUseInverseSquaredFalloff(bInverseSquaredFalloff);
		PointLight->SetLightFalloffExponent(LightFalloffExponent);
	}
}

void FSpotLightProperties::FromLightComponent(const ULightComponent* Target)
{
	FBaseLightProperties::FromLightComponent(Target);
	if (const USpotLightComponent* SpotLight = Cast<USpotLightComponent>(Target))
	{
		InnerConeAngle			= SpotLight->InnerConeAngle;
		OuterConeAngle			= SpotLight->OuterConeAngle;
		SourceRadius			= SpotLight->SourceRadius;
		SoftSourceRadius		= SpotLight->SoftSourceRadius;
		SourceLength			= SpotLight->SourceLength;
		bInverseSquaredFalloff	= SpotLight->bUseInverseSquaredFalloff;
		LightFalloffExponent	= SpotLight->LightFalloffExponent;
	}
}

void FSpotLightProperties::ToLightComponent(ULightComponent* Target) const
{
	FBaseLightProperties::ToLightComponent(Target);
	if (USpotLightComponent* SpotLight = Cast<USpotLightComponent>(Target))
	{
		SpotLight->SetInnerConeAngle(InnerConeAngle);
		SpotLight->SetOuterConeAngle(OuterConeAngle);
		SpotLight->SetSourceRadius(SourceRadius);
		SpotLight->SetSoftSourceRadius(SoftSourceRadius);
		SpotLight->SetSourceLength(SourceLength);
		SpotLight->SetUseInverseSquaredFalloff(bInverseSquaredFalloff);
		SpotLight->SetLightFalloffExponent(LightFalloffExponent);
	}
}

void FRectLightProperties::FromLightComponent(const ULightComponent* Target)
{
	FBaseLightProperties::FromLightComponent(Target);
	if (const URectLightComponent* RectLight = Cast<URectLightComponent>(Target))
	{
		SourceWidth		= RectLight->SourceWidth;
		SourceHeight	= RectLight->SourceHeight;
		BarnDoorAngle	= RectLight->BarnDoorAngle;
		BarnDoorLength	= RectLight->BarnDoorLength;
		SourceTexture	= RectLight->SourceTexture;
	}
}

void FRectLightProperties::ToLightComponent(ULightComponent* Target) const
{
	FBaseLightProperties::ToLightComponent(Target);
	if (URectLightComponent* RectLight = Cast<URectLightComponent>(Target))
	{
		RectLight->SetSourceWidth(SourceWidth);
		RectLight->SetSourceHeight(SourceHeight);
		RectLight->SetBarnDoorAngle(BarnDoorAngle);
		RectLight->SetBarnDoorLength(BarnDoorLength);
		RectLight->SetSourceTexture(SourceTexture.LoadSynchronous());
	}
}