// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "Engine/Scene.h"
#include "Components/LightComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/SpotLightComponent.h"
#include "Components/RectLightComponent.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "LightProperties.generated.h"

/**
 * Defines the culling and fading behavior for a light based on its distance from the camera.
 */
USTRUCT(BlueprintType)
struct TOROCORE_API FLightDrawDistance final
{
	GENERATED_BODY()

    /** If true, the light will be culled based on the MaxDistance. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DrawDistance)
		bool bUseDrawDistance;

    /** The maximum distance from the camera at which this light will be rendered. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DrawDistance, meta = (EditCondition = "bUseDrawDistance", ClampMin = 0.0f, UIMin = 0.0f))
		float MaxDistance;

    /** The distance range over which the light fades out as it approaches MaxDistance. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DrawDistance, meta = (EditCondition = "bUseDrawDistance"))
		float FadeRange;

	FLightDrawDistance(): bUseDrawDistance(true), MaxDistance(5000.0f), FadeRange(1000.0f) {}

	float GetMaxDistance() const { return bUseDrawDistance ? FMath::Max(0.0f, MaxDistance) : 0.0f; }
    float GetFadeRange() const { return bUseDrawDistance ? FMath::Max(0.0f, FadeRange) : 0.0f; }

    void FromLightComponent(const ULightComponent* Target);
    void ToLightComponent(ULightComponent* Target) const;
};

/**
 * Base structure for common lighting properties used across all light types.
 */
USTRUCT(BlueprintType)
struct TOROCORE_API FBaseLightProperties
{
    GENERATED_BODY()

    /** The units used to interpret the Intensity value (Candelas, Lumens, etc.). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties, meta = (DisplayPriority = 0))
        ELightUnits IntensityUnits;

    /** Total energy emitted by the light. Interpretation depends on IntensityUnits. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties, meta = (ClampMin = 0.0f, UIMin = 0.0f, DisplayPriority = 0))
        float Intensity;

    /** The color emitted by the light. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties, meta = (HideAlphaChannel, DisplayPriority = 0))
        FLinearColor Color;

    /** The radius of the light's influence. Only applicable to Local Lights. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties, meta = (ClampMin = 0.0f, UIMin = 8.0f, UIMax = 16384.0f, DisplayPriority = 0))
        float AttenuationRadius;

    /** Toggle to enable Correlated Color Temperature (Kelvin). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties, meta = (InlineEditConditionToggle, DisplayPriority = 2))
        bool bUseTemperature;

    /** Color temperature in Kelvin. Lower values are warmer (red), higher are cooler (blue). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties, meta = (UIMin = 1700.0f, UIMax = 12000.0f, EditCondition = "bUseTemperature", DisplayPriority = 2))
        float Temperature;

    /** Scales the contribution of this light to the indirect lighting buffer (GI). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties, meta = (UIMin = 0.0f, UIMax = 6.0f, DisplayPriority = 2))
        float IndirectIntensity;

    /** Scales the contribution of this light to volumetric effects like fog. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties, meta = (UIMin = 0.25f, UIMax = 4.0f, DisplayName = "Volumetric Scattering Intensity", DisplayPriority = 2))
        float VolumetricIntensity;

    /** Whether this light should cast dynamic shadows. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties, meta = (DisplayPriority = 2))
        bool bLightCastShadows;

    /** Whether this light should cast shadows within volumetric fog. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties, meta = (DisplayName = "Cast Volumetric Shadows", DisplayPriority = 2))
        bool bVolumetricShadows;

    /** Scales the specular highlight intensity from this light. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties, meta = (ClampMin = 0.0f, ClampMax = 1.0f, UIMin = 0.0f, UIMax = 1.0f, DisplayPriority = 3))
        float SpecularScale;

    /** If enabled, this light can be processed by the MegaLights system (Lumen). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties, AdvancedDisplay, meta = (DisplayPriority = 0))
        bool bAllowMegaLights;

    /** Defines which lighting channels this light affects. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties, AdvancedDisplay, meta = (DisplayPriority = 0))
        FLightingChannels LightChannels;

    /** Distance-based culling settings for this light. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties, AdvancedDisplay, meta = (DisplayPriority = 0))
        FLightDrawDistance DrawDistance;

    FBaseLightProperties()
        : IntensityUnits(ELightUnits::Candelas)
        , Intensity(8.0f)
        , Color(FLinearColor::White)
        , AttenuationRadius(1000.0f)
        , bUseTemperature(false)
        , Temperature(6500.0f)
        , IndirectIntensity(1.0f)
        , VolumetricIntensity(1.0f)
        , bLightCastShadows(true)
        , bVolumetricShadows(false)
        , SpecularScale(1.0f)
        , bAllowMegaLights(false)
    {}

    virtual ~FBaseLightProperties() = default;

    void CopyFrom(const FBaseLightProperties& Properties);

    virtual void FromLightComponent(const ULightComponent* Target);
    virtual void ToLightComponent(ULightComponent* Target) const;
};

/**
 * Specialized properties for PointLights, which emit light in all directions from a single point.
 */
USTRUCT(BlueprintType)
struct TOROCORE_API FPointLightProperties final : public FBaseLightProperties
{
    GENERATED_BODY()

    /** Radius of the light source shape. Larger values create softer shadows. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties, meta = (ClampMin = 0.0f, UIMin = 0.0f, DisplayPriority = 1))
        float SourceRadius;

    /** Softness of the light source edge. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties, meta = (ClampMin = 0.0f, UIMin = 0.0f, DisplayPriority = 1))
        float SoftSourceRadius;

    /** Length of the light source shape, transforming the point into a capsule light. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties, meta = (ClampMin = 0.0f, UIMin = 0.0f, DisplayPriority = 1))
        float SourceLength;

    /** Whether to use physically-correct inverse squared falloff. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties, meta = (InlineEditConditionToggle, DisplayPriority = 3))
        bool bInverseSquaredFalloff;

    /** Custom falloff exponent used only if bInverseSquaredFalloff is disabled. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties, meta = (UIMin = 2.0f, UIMax = 16.0f, EditCondition = "!bInverseSquaredFalloff", DisplayPriority = 3))
        float LightFalloffExponent;

    FPointLightProperties()
        : SourceRadius(0.0f)
        , SoftSourceRadius(0.0f)
        , SourceLength(0.0f)
        , bInverseSquaredFalloff(true)
        , LightFalloffExponent(8.0f)
    {}

    virtual void FromLightComponent(const ULightComponent* Target) override;
    virtual void ToLightComponent(ULightComponent* Target) const override;
};

/**
 * Specialized properties for SpotLights, emitting light in a cone-shaped volume.
 */
USTRUCT(BlueprintType)
struct TOROCORE_API FSpotLightProperties final : public FBaseLightProperties
{
    GENERATED_BODY()

    /** The inner cone angle in degrees. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties, meta = (UIMin = 1.0f, UIMax = 80.0f, DisplayPriority = 1))
        float InnerConeAngle;

    /** The outer cone angle in degrees. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties, meta = (UIMin = 1.0f, UIMax = 80.0f, DisplayPriority = 1))
        float OuterConeAngle;

    /** Radius of the light source shape. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties, meta = (ClampMin = 0.0f, UIMin = 0.0f, DisplayPriority = 1))
        float SourceRadius;

    /** Softness of the light source edge. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties, meta = (ClampMin = 0.0f, UIMin = 0.0f, DisplayPriority = 1))
        float SoftSourceRadius;

    /** Length of the light source shape. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties, meta = (ClampMin = 0.0f, UIMin = 0.0f, DisplayPriority = 1))
        float SourceLength;

    /** Whether to use physically-correct inverse squared falloff. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties, meta = (InlineEditConditionToggle, DisplayPriority = 3))
        bool bInverseSquaredFalloff;

    /** Custom falloff exponent used only if bInverseSquaredFalloff is disabled. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties, meta = (UIMin = 2.0f, UIMax = 16.0f, EditCondition = "!bInverseSquaredFalloff", DisplayPriority = 3))
        float LightFalloffExponent;

    FSpotLightProperties()
        : InnerConeAngle(0.0f)
        , OuterConeAngle(44.0f)
        , SourceRadius(0.0f)
        , SoftSourceRadius(0.0f)
        , SourceLength(0.0f)
        , bInverseSquaredFalloff(true)
        , LightFalloffExponent(8.0f)
    {}

    virtual void FromLightComponent(const ULightComponent* Target) override;
    virtual void ToLightComponent(ULightComponent* Target) const override;
};

/**
 * Specialized properties for RectLights, emitting light from a rectangular surface.
 */
USTRUCT(BlueprintType)
struct TOROCORE_API FRectLightProperties final : public FBaseLightProperties
{
    GENERATED_BODY()

    /** Width of the rectangular light source. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties, meta = (ClampMin = 10.0f, UIMin = 10.0f, DisplayPriority = 1))
        float SourceWidth;

    /** Height of the rectangular light source. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties, meta = (ClampMin = 10.0f, UIMin = 10.0f, DisplayPriority = 1))
        float SourceHeight;

    /** Angle of the barn doors attached to the rect light. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties, meta = (ClampMin = 0.0f, ClampMax = 90.0f, UIMin = 0.0f, UIMax = 90.0f, DisplayPriority = 1))
        float BarnDoorAngle;

    /** Length of the barn doors. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties, meta = (ClampMin = 1.0f, UIMin = 1.0f, DisplayPriority = 1))
        float BarnDoorLength;

    /** Texture to apply to the light source surface. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties, meta = (DisplayPriority = 1))
        TSoftObjectPtr<UTexture> SourceTexture;

    FRectLightProperties()
        : SourceWidth(64.0f)
        , SourceHeight(64.0f)
        , BarnDoorAngle(88.0f)
        , BarnDoorLength(20.0f)
        , SourceTexture(nullptr)
    {}

    virtual void FromLightComponent(const ULightComponent* Target) override;
    virtual void ToLightComponent(ULightComponent* Target) const override;
};

/**
 * Blueprint function wrappers for LightProperties series of structs
 */
UCLASS(NotBlueprintable, NotBlueprintType)
class TOROCORE_API ULightPropertiesLibrary final : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:

	/** 
	 * Applies distance-based culling and fading settings to a Light Component.
	 * @param Target The light component to update.
	 * @param Settings The draw distance configuration to apply.
	 */
	UFUNCTION(BlueprintCallable, Category = "Rendering|Components|Light", meta = (DefaultToSelf = "Target"))
	static void SetLightDrawDistanceSettings(ULightComponent* Target, const FLightDrawDistance& Settings)
	{
		Settings.ToLightComponent(Target);
	}

	/** 
	 * Extracts the current draw distance and fading settings from a Light Component.
	 * @param OutData The struct to receive the extracted draw distance data (Output).
	 * @param Target The source light component to read from.
	 */
	UFUNCTION(BlueprintPure, Category = "Rendering|Components|Light", meta = (DefaultToSelf = "Target"))
	static void GetLightDrawDistanceSettings(FLightDrawDistance& OutData, const ULightComponent* Target)
	{
		OutData.FromLightComponent(Target);
	}

	/** 
	 * Applies common base lighting properties (Intensity, Color, Attenuation, etc.) to any Light Component.
	 * @param Target The light component to update.
	 * @param Properties The base properties to apply.
	 */
	UFUNCTION(BlueprintCallable, Category = "Rendering|Components|Light", meta = (DefaultToSelf = "Target"))
	static void SetBaseLightProperties(ULightComponent* Target, const FBaseLightProperties& Properties)
	{
		Properties.ToLightComponent(Target);
	}

	/** 
	 * Extracts common base lighting properties from any Light Component.
	 * @param OutData The struct to receive the base property data (Output).
	 * @param Target The source light component to read from.
	 */
	UFUNCTION(BlueprintPure, Category = "Rendering|Components|Light", meta = (DefaultToSelf = "Target"))
	static void GetBaseLightProperties(FBaseLightProperties& OutData, const ULightComponent* Target)
	{
		OutData.FromLightComponent(Target);
	}

	/** 
	 * Applies point-light specific properties (Source Radius, Falloff, etc.) to a Point Light Component.
	 * @param Target The point light component to update.
	 * @param Properties The specialized point light properties to apply.
	 */
	UFUNCTION(BlueprintCallable, Category = "Rendering|Components|Light", meta = (DefaultToSelf = "Target"))
	static void SetPointLightProperties(UPointLightComponent* Target, const FPointLightProperties& Properties)
	{
		Properties.ToLightComponent(Target);
	}

	/** 
	 * Extracts specialized point-light properties from a Point Light Component.
	 * @param OutData The struct to receive the point light property data (Output).
	 * @param Target The source point light component to read from.
	 */
	UFUNCTION(BlueprintPure, Category = "Rendering|Components|Light", meta = (DefaultToSelf = "Target"))
	static void GetPointLightProperties(FPointLightProperties& OutData, const UPointLightComponent* Target)
	{
		OutData.FromLightComponent(Target);
	}

	/** 
	 * Applies spot-light specific properties (Cone Angles, Source Radius, etc.) to a Spot Light Component.
	 * @param Target The spot light component to update.
	 * @param Properties The specialized spot light properties to apply.
	 */
	UFUNCTION(BlueprintCallable, Category = "Rendering|Components|Light", meta = (DefaultToSelf = "Target"))
	static void SetSpotLightProperties(USpotLightComponent* Target, const FSpotLightProperties& Properties)
	{
		Properties.ToLightComponent(Target);
	}

	/** 
	 * Extracts specialized spot-light properties from a Spot Light Component.
	 * @param OutData The struct to receive the spot light property data (Output).
	 * @param Target The source spot light component to read from.
	 */
	UFUNCTION(BlueprintPure, Category = "Rendering|Components|Light", meta = (DefaultToSelf = "Target"))
	static void GetSpotLightProperties(FSpotLightProperties& OutData, const USpotLightComponent* Target)
	{
		OutData.FromLightComponent(Target);
	}

	/** 
	 * Applies rect-light specific properties (Width, Height, Barn Doors, etc.) to a Rect Light Component.
	 * @param Target The rect light component to update.
	 * @param Properties The specialized rect light properties to apply.
	 */
	UFUNCTION(BlueprintCallable, Category = "Rendering|Components|Light", meta = (DefaultToSelf = "Target"))
	static void SetRectLightProperties(URectLightComponent* Target, const FRectLightProperties& Properties)
	{
		Properties.ToLightComponent(Target);
	}

	/** 
	 * Extracts specialized rect-light properties from a Rect Light Component.
	 * @param OutData The struct to receive the rect light property data (Output).
	 * @param Target The source rect light component to read from.
	 */
	UFUNCTION(BlueprintPure, Category = "Rendering|Components|Light", meta = (DefaultToSelf = "Target"))
	static void GetRectLightProperties(FRectLightProperties& OutData, const URectLightComponent* Target)
	{
		OutData.FromLightComponent(Target);
	}
};