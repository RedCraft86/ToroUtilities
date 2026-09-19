// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#include "DataTypes/LightSettings.h"
#include "Components/LightComponent.h"
#include "Components/LocalLightComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/RectLightComponent.h"
#include "Components/SpotLightComponent.h"

float FLightDrawDistanceSettings::GetMaxDistance() const
{
	return bUseDrawDistance ? FMath::Max(0.0f, MaxDistance) : 0.0f;
}

float FLightDrawDistanceSettings::GetFadeRange() const
{
	return bUseDrawDistance ? FMath::Max(0.0f, FadeRange) : 0.0f;
}

void FLightDrawDistanceSettings::UpdateSettings()
{
	if (FadeRange > MaxDistance)
	{
		FadeRange = MaxDistance;
	}
}

void FLightDrawDistanceSettings::FromLightComponent(const ULightComponent* Target)
{
	if (Target)
	{
		MaxDistance	= Target->MaxDrawDistance;
		FadeRange	= Target->MaxDistanceFadeRange;
		bUseDrawDistance = MaxDistance > UE_SMALL_NUMBER || FadeRange > UE_SMALL_NUMBER;
	}
}

void FLightDrawDistanceSettings::ToLightComponent(ULightComponent* Target) const
{
	if (Target)
	{
		Target->SetMaxDrawDistance(GetMaxDistance());
		Target->SetMaxDistanceFadeRange(GetFadeRange());
	}
}

void FBaseLightSettings::CopyFrom(const FBaseLightSettings& Settings)
{
	IntensityUnits		= Settings.IntensityUnits;
	Intensity			= Settings.Intensity;
	Color				= Settings.Color;
	AttenuationRadius	= Settings.AttenuationRadius;
	bUseTemperature		= Settings.bUseTemperature;
	Temperature			= Settings.Temperature;
	IndirectIntensity	= Settings.IndirectIntensity;
	VolumetricIntensity	= Settings.VolumetricIntensity;
	bCastShadows		= Settings.bCastShadows;
	bVolumetricShadows	= Settings.bVolumetricShadows;
	SpecularScale		= Settings.SpecularScale;
	bAllowMegaLights	= Settings.bAllowMegaLights;
	LightChannels		= Settings.LightChannels;
	DrawDistance		= Settings.DrawDistance;
}

void FBaseLightSettings::UpdateSettings()
{
	DrawDistance.UpdateSettings();
}

void FBaseLightSettings::FromLightComponent(const ULightComponent* Target)
{
	if (Target)
	{
		Intensity			= Target->Intensity;
		Color				= Target->GetLightColor();
		bUseTemperature		= Target->bUseTemperature;
		Temperature			= Target->Temperature;
		IndirectIntensity	= Target->IndirectLightingIntensity;
		VolumetricIntensity	= Target->VolumetricScatteringIntensity;
		bCastShadows		= Target->CastShadows;
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

	UpdateSettings();
}

void FBaseLightSettings::ToLightComponent(ULightComponent* Target) const
{
	if (Target)
	{
		Target->SetIntensity(Intensity);
		Target->SetLightColor(Color);
		Target->SetTemperature(Temperature); // Before SetUseTemperature to avoid overriding
		Target->SetUseTemperature(bUseTemperature);
		Target->SetIndirectLightingIntensity(IndirectIntensity);
		Target->SetVolumetricScatteringIntensity(VolumetricIntensity);
		Target->SetCastShadows(bCastShadows);
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

void FPointLightSettings::UpdateSettings()
{
	FBaseLightSettings::UpdateSettings();

	if (bManualFalloff)
	{
		IntensityUnits = ELightUnits::Unitless;
	}
}

void FPointLightSettings::FromLightComponent(const ULightComponent* Target)
{
	if (const UPointLightComponent* PointLight = Cast<UPointLightComponent>(Target))
	{
		SourceRadius		= PointLight->SourceRadius;
		SoftSourceRadius	= PointLight->SoftSourceRadius;
		SourceLength		= PointLight->SourceLength;
		bManualFalloff		= !PointLight->bUseInverseSquaredFalloff;
		FalloffExponent		= PointLight->LightFalloffExponent;
	}

	FBaseLightSettings::FromLightComponent(Target);
}

void FPointLightSettings::ToLightComponent(ULightComponent* Target) const
{
	FBaseLightSettings::ToLightComponent(Target);
	if (UPointLightComponent* PointLight = Cast<UPointLightComponent>(Target))
	{
		PointLight->SetSourceRadius(SourceRadius);
		PointLight->SetSoftSourceRadius(SoftSourceRadius);
		PointLight->SetSourceLength(SourceLength);
		PointLight->SetUseInverseSquaredFalloff(!bManualFalloff);
		PointLight->SetLightFalloffExponent(FalloffExponent);
	}
}

void FSpotLightSettings::UpdateSettings()
{
	FBaseLightSettings::UpdateSettings();

	if (OuterConeAngle < InnerConeAngle)
	{
		OuterConeAngle = InnerConeAngle;
	}

	if (bManualFalloff)
	{
		IntensityUnits = ELightUnits::Unitless;
	}
}

void FSpotLightSettings::FromLightComponent(const ULightComponent* Target)
{
	if (const USpotLightComponent* SpotLight = Cast<USpotLightComponent>(Target))
	{
		InnerConeAngle		= SpotLight->InnerConeAngle;
		OuterConeAngle		= SpotLight->OuterConeAngle;
		SourceRadius		= SpotLight->SourceRadius;
		SoftSourceRadius	= SpotLight->SoftSourceRadius;
		SourceLength		= SpotLight->SourceLength;
		bManualFalloff		= !SpotLight->bUseInverseSquaredFalloff;
		FalloffExponent		= SpotLight->LightFalloffExponent;
	}

	FBaseLightSettings::FromLightComponent(Target);
}

void FSpotLightSettings::ToLightComponent(ULightComponent* Target) const
{
	FBaseLightSettings::ToLightComponent(Target);
	if (USpotLightComponent* SpotLight = Cast<USpotLightComponent>(Target))
	{
		SpotLight->SetInnerConeAngle(InnerConeAngle);
		SpotLight->SetOuterConeAngle(OuterConeAngle);
		SpotLight->SetSourceRadius(SourceRadius);
		SpotLight->SetSoftSourceRadius(SoftSourceRadius);
		SpotLight->SetSourceLength(SourceLength);
		SpotLight->SetUseInverseSquaredFalloff(!bManualFalloff);
		SpotLight->SetLightFalloffExponent(FalloffExponent);
	}
}

void FRectLightSettings::UpdateSettings()
{
	FBaseLightSettings::UpdateSettings();
}

void FRectLightSettings::FromLightComponent(const ULightComponent* Target)
{
	if (const URectLightComponent* RectLight = Cast<URectLightComponent>(Target))
	{
		SourceWidth		= RectLight->SourceWidth;
		SourceHeight	= RectLight->SourceHeight;
		BarnDoorAngle	= RectLight->BarnDoorAngle;
		BarnDoorLength	= RectLight->BarnDoorLength;
		SourceTexture	= RectLight->SourceTexture;
	}

	FBaseLightSettings::FromLightComponent(Target);
}

void FRectLightSettings::ToLightComponent(ULightComponent* Target) const
{
	FBaseLightSettings::ToLightComponent(Target);
	if (URectLightComponent* RectLight = Cast<URectLightComponent>(Target))
	{
		RectLight->SetSourceWidth(SourceWidth);
		RectLight->SetSourceHeight(SourceHeight);
		RectLight->SetBarnDoorAngle(BarnDoorAngle);
		RectLight->SetBarnDoorLength(BarnDoorLength);
		RectLight->SetSourceTexture(SourceTexture.LoadSynchronous());
	}
}

void ULightSettingsLibrary::SetLightDrawDistanceSettings(ULightComponent* Target, const FLightDrawDistanceSettings& Settings)
{
	Settings.ToLightComponent(Target);
}

void ULightSettingsLibrary::GetLightDrawDistanceSettings(FLightDrawDistanceSettings& OutData, const ULightComponent* Target)
{
	OutData.FromLightComponent(Target);
}

void ULightSettingsLibrary::UpdateBaseLightSettings(FBaseLightSettings& Settings)
{
	Settings.UpdateSettings();
}

void ULightSettingsLibrary::SetBaseLightSettings(ULightComponent* Target, const FBaseLightSettings& Settings)
{
	Settings.ToLightComponent(Target);
}

void ULightSettingsLibrary::GetBaseLightSettings(FBaseLightSettings& OutData, const ULightComponent* Target)
{
	OutData.FromLightComponent(Target);
}

void ULightSettingsLibrary::UpdatePointLightSettings(FPointLightSettings& Settings)
{
	Settings.UpdateSettings();
}

void ULightSettingsLibrary::SetPointLightSettings(UPointLightComponent* Target, const FPointLightSettings& Settings)
{
	Settings.ToLightComponent(Target);
}

void ULightSettingsLibrary::GetPointLightSettings(FPointLightSettings& OutData, const UPointLightComponent* Target)
{
	OutData.FromLightComponent(Target);
}

void ULightSettingsLibrary::UpdateSpotLightSettings(FSpotLightSettings& Settings)
{
	Settings.UpdateSettings();
}

void ULightSettingsLibrary::SetSpotLightSettings(USpotLightComponent* Target, const FSpotLightSettings& Settings)
{
	Settings.ToLightComponent(Target);
}

void ULightSettingsLibrary::GetSpotLightSettings(FSpotLightSettings& OutData, const USpotLightComponent* Target)
{
	OutData.FromLightComponent(Target);
}

void ULightSettingsLibrary::UpdateRectLightSettings(FRectLightSettings& Settings)
{
	Settings.UpdateSettings();
}

void ULightSettingsLibrary::SetRectLightSettings(URectLightComponent* Target, const FRectLightSettings& Settings)
{
	Settings.ToLightComponent(Target);
}

void ULightSettingsLibrary::GetRectLightSettings(FRectLightSettings& OutData, const URectLightComponent* Target)
{
	OutData.FromLightComponent(Target);
}
