// Copyright (C) RedCraft86. All Rights Reserved.

#include "Libraries/ToroLightingUtils.h"
#include "Components/LocalLightComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/SpotLightComponent.h"
#include "Components/RectLightComponent.h"
#include "Components/LightComponent.h"

FBaseLightProperties UToroLightingUtils::GetBaseProperties_Point(const FPointLightProperties& Target)
{
	return Target.GetBase();
}

FBaseLightProperties UToroLightingUtils::GetBaseProperties_Spot(const FSpotLightProperties& Target)
{
	return Target.GetBase();
}

FBaseLightProperties UToroLightingUtils::GetBaseProperties_Rect(const FRectLightProperties& Target)
{
	return Target.GetBase();
}

FPointLightProperties UToroLightingUtils::GetPointProperties(const FBaseLightProperties& Target, const bool bBasicData)
{
	FPointLightProperties Result;
	if (bBasicData) Result.CopyBaseData(Target);
	else Result.CopyFrom(Target);
	return Result;
}

FSpotLightProperties UToroLightingUtils::GetSpotProperties(const FBaseLightProperties& Target, const bool bBasicData)
{
	FSpotLightProperties Result;
	if (bBasicData) Result.CopyBaseData(Target);
	else Result.CopyFrom(Target);
	return Result;
}

FRectLightProperties UToroLightingUtils::GetRectProperties(const FBaseLightProperties& Target, const bool bBasicData)
{
	FRectLightProperties Result;
	if (bBasicData) Result.CopyBaseData(Target);
	else Result.CopyFrom(Target);
	return Result;
}

void UToroLightingUtils::SetLightDrawDistanceSettings(ULightComponent* Target, const FLightDrawDistance& Settings)
{
	if (IsValid(Target))
	{
		Target->MaxDrawDistance = Settings.GetMaxDrawDistance();
		Target->MaxDistanceFadeRange = Settings.GetMaxDistanceFadeRange();
	}
}

FLightDrawDistance UToroLightingUtils::GetLightDrawDistanceSettings(const ULightComponent* Target)
{
	FLightDrawDistance Result;
	if (IsValid(Target))
	{
		Result.MaxDrawDistance = Target->MaxDrawDistance;
		Result.MaxDistanceFadeRange = Target->MaxDistanceFadeRange;
		Result.bUseDrawDistance = Result.IsValidData();
	}
	return Result;
}

void UToroLightingUtils::SetBaseLightProperties(ULightComponent* Target, const FBaseLightProperties& Properties)
{
	if (IsValid(Target))
	{
		Target->SetIntensity(Properties.Intensity);
		Target->SetLightColor(Properties.Color);
		Target->SetUseTemperature(Properties.bUseTemperature);
		Target->SetTemperature(Properties.Temperature);
		Target->SetIndirectLightingIntensity(Properties.IndirectIntensity);
		Target->SetVolumetricScatteringIntensity(Properties.VolumetricScatteringIntensity);
		Target->SetCastShadows(Properties.bLightCastShadows);
		Target->SetCastVolumetricShadow(Properties.bCastVolumetricShadows);
		Target->SetSpecularScale(Properties.SpecularScale);

		if (ULocalLightComponent* LocalLight = Cast<ULocalLightComponent>(Target))
		{
			LocalLight->SetIntensityUnits(Properties.IntensityUnits);
			LocalLight->SetAttenuationRadius(Properties.AttenuationRadius);
		}

		SetLightDrawDistanceSettings(Target, Properties.DrawDistance);
	}
}

FBaseLightProperties UToroLightingUtils::GetBaseLightProperties(const ULightComponent* Target)
{
	FBaseLightProperties OutData;
	if (IsValid(Target))
	{
		OutData.Intensity = Target->Intensity;
		OutData.Color = Target->LightColor;
		OutData.bUseTemperature = Target->bUseTemperature;
		OutData.Temperature = Target->Temperature;
		OutData.IndirectIntensity = Target->IndirectLightingIntensity;
		OutData.VolumetricScatteringIntensity = Target->VolumetricScatteringIntensity;
		OutData.bLightCastShadows = Target->CastShadows;
		OutData.bCastVolumetricShadows = Target->bCastVolumetricShadow;
		OutData.SpecularScale = Target->SpecularScale;

		if (const ULocalLightComponent* LocalLight = Cast<ULocalLightComponent>(Target))
		{
			OutData.IntensityUnits = LocalLight->IntensityUnits;
			OutData.AttenuationRadius = LocalLight->AttenuationRadius;
		}

		OutData.DrawDistance = GetLightDrawDistanceSettings(Target);
	}
	return OutData;
}

void UToroLightingUtils::SetPointLightProperties(UPointLightComponent* Target, const FPointLightProperties& Properties)
{
	if (IsValid(Target))
	{
		SetBaseLightProperties(Target, Properties);
		Target->SetSourceRadius(Properties.SourceRadius);
		Target->SetSoftSourceRadius(Properties.SoftSourceRadius);
		Target->SetSourceLength(Properties.SourceLength);
		Target->bUseInverseSquaredFalloff = Properties.bUseInverseSquaredFalloff;
		Target->SetLightFalloffExponent(Properties.LightFalloffExponent);
	}
}

FPointLightProperties UToroLightingUtils::GetPointLightProperties(const UPointLightComponent* Target)
{
	FPointLightProperties OutData;
	if (Target)
	{
		OutData.CopyFrom(GetBaseLightProperties(Target));
		OutData.SourceRadius = Target->SourceRadius;
		OutData.SoftSourceRadius = Target->SoftSourceRadius;
		OutData.SourceLength = Target->SourceLength;
		OutData.bUseInverseSquaredFalloff = Target->bUseInverseSquaredFalloff;
		OutData.LightFalloffExponent = Target->LightFalloffExponent;
	}
	return OutData;
}

void UToroLightingUtils::SetSpotLightProperties(USpotLightComponent* Target, const FSpotLightProperties& Properties)
{
	if (IsValid(Target))
	{
		SetBaseLightProperties(Target, Properties);
		Target->SetInnerConeAngle(Properties.InnerConeAngle);
		Target->SetOuterConeAngle(Properties.OuterConeAngle);
		Target->SetSourceRadius(Properties.SourceRadius);
		Target->SetSoftSourceRadius(Properties.SoftSourceRadius);
		Target->SetSourceLength(Properties.SourceLength);
		Target->bUseInverseSquaredFalloff = Properties.bUseInverseSquaredFalloff;
		Target->SetLightFalloffExponent(Properties.LightFalloffExponent);
	}
}

FSpotLightProperties UToroLightingUtils::GetSpotLightProperties(const USpotLightComponent* Target)
{
	FSpotLightProperties OutData;
	if (Target)
	{
		OutData.CopyFrom(GetBaseLightProperties(Target));
		OutData.InnerConeAngle = Target->InnerConeAngle;
		OutData.OuterConeAngle = Target->OuterConeAngle;
		OutData.SourceRadius = Target->SourceRadius;
		OutData.SoftSourceRadius = Target->SoftSourceRadius;
		OutData.SourceLength = Target->SourceLength;
		OutData.bUseInverseSquaredFalloff = Target->bUseInverseSquaredFalloff;
		OutData.LightFalloffExponent = Target->LightFalloffExponent;
	}
	return OutData;
}

void UToroLightingUtils::SetRectLightProperties(URectLightComponent* Target, const FRectLightProperties& Properties)
{
	if (IsValid(Target))
	{
		SetBaseLightProperties(Target, Properties);
		Target->SetSourceWidth(Properties.SourceWidth);
		Target->SetSourceHeight(Properties.SourceHeight);
		Target->SetBarnDoorAngle(Properties.BarnDoorAngle);
		Target->SetBarnDoorLength(Properties.BarnDoorLength);
		Target->SetSourceTexture(Properties.SourceTexture);
	}
}

FRectLightProperties UToroLightingUtils::GetRectLightProperties(const URectLightComponent* Target)
{
	FRectLightProperties OutData;
	if (Target)
	{
		OutData.CopyFrom(GetBaseLightProperties(Target));
		OutData.SourceWidth = Target->SourceWidth;
		OutData.SourceHeight = Target->SourceHeight;
		OutData.BarnDoorAngle = Target->BarnDoorAngle;
		OutData.BarnDoorLength = Target->BarnDoorLength;
		OutData.SourceTexture = Target->SourceTexture;
	}
	return OutData;
}
