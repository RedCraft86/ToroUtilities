// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "LightSettings.generated.h"

class URectLightComponent;
class USpotLightComponent;
class UPointLightComponent;

/**
 * Controls the maximum draw distance and fade range of a light component.
 * Disabled settings write zero for both distances.
 */
USTRUCT(BlueprintType)
struct TORORUNTIME_API FLightDrawDistanceSettings final
{
	GENERATED_BODY()

	/**
	 * Enables the draw distance and fade settings.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DrawDistance)
	bool bUseDrawDistance;

	/**
	 * Maximum distance at which the light is drawn.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DrawDistance, meta = (EditCondition = bUseDrawDistance, ClampMin = 0.0f))
	float MaxDistance;

	/**
	 * Distance over which the light fades before reaching its maximum distance.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DrawDistance, meta = (EditCondition = bUseDrawDistance, ClampMin = 0.0f))
	float FadeRange;

	FLightDrawDistanceSettings()
		: bUseDrawDistance(false), MaxDistance(5000.0f), FadeRange(1000.0f)
	{}

	float GetMaxDistance() const;
	float GetFadeRange() const;

	void UpdateSettings();
	void FromLightComponent(const ULightComponent* Target);
	void ToLightComponent(ULightComponent* Target) const;
};

/**
 * Stores settings shared by light components, including local light settings.
 * Intensity units and attenuation radius are read or written only for local lights.
 */
USTRUCT(BlueprintType)
struct TORORUNTIME_API FBaseLightSettings
{
	GENERATED_BODY()

	/**
	 * Units used to interpret intensity on local light components.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (DisplayPriority = 0))
	ELightUnits IntensityUnits;

	/**
	 * Brightness of the light in the selected intensity units.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (ClampMin = 0.0f, UIMin = 0.0f, DisplayPriority = 0))
	float Intensity;

	/**
	 * Color of the light. Alpha channel has no effect and is hidden in the editor.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (HideAlphaChannel, DisplayPriority = 0))
	FLinearColor Color;

	/**
	 * Maximum influence radius of a local light.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (ClampMin = 0.0f, UIMin = 8.0f, UIMax = 16384.0f, DisplayPriority = 0))
	float AttenuationRadius;

	/**
	 * Enables color temperature in addition to the light color.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (InlineEditConditionToggle, DisplayPriority = 2))
	bool bUseTemperature;

	/**
	 * Color temperature in kelvin when temperature is enabled.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (EditCondition = bUseTemperature, UIMin = 1700.0f, UIMax = 12000.0f, DisplayPriority = 2))
	float Temperature;

	/**
	 * Multiplier for the light's indirect lighting contribution.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (UIMin = 0.0f, UIMax = 6.0f, DisplayPriority = 2))
	float IndirectIntensity;

	/**
	 * Multiplier for the light's volumetric scattering contribution.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (UIMin = 0.0f, UIMax = 4.0f, DisplayPriority = 2))
	float VolumetricIntensity;

	/**
	 * Whether the light casts shadows.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (DisplayPriority = 2))
	bool bCastShadows;

	/**
	 * Whether the light casts shadows into volumetric fog.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (DisplayPriority = 2))
	bool bVolumetricShadows;

	/**
	 * Multiplier for the light's specular contribution.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (ClampMin = 0.0f, ClampMax = 1.0f, UIMin = 0.0f, UIMax = 1.0f, DisplayPriority = 3))
	float SpecularScale;

	/**
	 * Whether the light can participate in MegaLights.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, AdvancedDisplay, meta = (DisplayPriority = 0))
	bool bAllowMegaLights;

	/**
	 * Lighting channels that receive the light.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, AdvancedDisplay, meta = (DisplayPriority = 0))
	FLightingChannels LightChannels;

	/**
	 * Maximum draw distance and fade settings for the light.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, AdvancedDisplay, meta = (DisplayPriority = 0))
	FLightDrawDistanceSettings DrawDistance;

	FBaseLightSettings()
		: IntensityUnits(ELightUnits::Candelas)
		, Intensity(8.0f)
		, Color(FLinearColor::White)
		, AttenuationRadius(1000.0f)
		, bUseTemperature(false)
		, Temperature(6500.0f)
		, IndirectIntensity(1.0f)
		, VolumetricIntensity(1.0f)
		, bCastShadows(true)
		, bVolumetricShadows(false)
		, SpecularScale(1.0f)
		, bAllowMegaLights(false)
	{}

	virtual ~FBaseLightSettings() = default;

	void CopyFrom(const FBaseLightSettings& Settings);

	virtual void UpdateSettings();
	virtual void FromLightComponent(const ULightComponent* Target);
	virtual void ToLightComponent(ULightComponent* Target) const;
};

/**
 * Stores the shared and point light specific settings of a point light.
 */
USTRUCT(BlueprintType)
struct TORORUNTIME_API FPointLightSettings final : public FBaseLightSettings
{
	GENERATED_BODY()

	/**
	 * Radius of the light source used for reflections and shadow softness.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (ClampMin = 0.0f, UIMin = 0.0f, DisplayPriority = 1))
	float SourceRadius;

	/**
	 * Additional soft radius of the light source.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (ClampMin = 0.0f, UIMin = 0.0f, DisplayPriority = 1))
	float SoftSourceRadius;

	/**
	 * Length of the light source.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (ClampMin = 0.0f, UIMin = 0.0f, DisplayPriority = 1))
	float SourceLength;

	/**
	 * Uses the falloff exponent instead of inverse squared falloff when enabled.
	 * Will also change intensity units to Unitless.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (InlineEditConditionToggle, DisplayPriority = 3))
	bool bManualFalloff;

	/**
	 * Falloff exponent used when manual falloff is enabled.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (EditCondition = bManualFalloff, UIMin = 2.0f, UIMax = 16.0f, DisplayPriority = 3))
	float FalloffExponent;

	FPointLightSettings()
		: SourceRadius(0.0f)
		, SoftSourceRadius(0.0f)
		, SourceLength(0.0f)
		, bManualFalloff(false)
		, FalloffExponent(8.0f)
	{}

	virtual void UpdateSettings() override;
	virtual void FromLightComponent(const ULightComponent* Target) override;
	virtual void ToLightComponent(ULightComponent* Target) const override;
};

/**
 * Stores the shared and spot light specific settings of a spot light.
 */
USTRUCT(BlueprintType)
struct TORORUNTIME_API FSpotLightSettings final : public FBaseLightSettings
{
	GENERATED_BODY()

	/**
	 * Angle in degrees of the fully illuminated inner cone.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (UIMin = 1.0f, UIMax = 80.0f, DisplayPriority = 1))
	float InnerConeAngle;

	/**
	 * Angle in degrees of the outer cone, where light falls to zero.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (UIMin = 1.0f, UIMax = 80.0f, DisplayPriority = 1))
	float OuterConeAngle;

	/**
	 * Radius of the light source used for reflections and shadow softness.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (ClampMin = 0.0f, UIMin = 0.0f, DisplayPriority = 1))
	float SourceRadius;

	/**
	 * Additional soft radius of the light source.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (ClampMin = 0.0f, UIMin = 0.0f, DisplayPriority = 1))
	float SoftSourceRadius;

	/**
	 * Length of the light source.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (ClampMin = 0.0f, UIMin = 0.0f, DisplayPriority = 1))
	float SourceLength;

	/**
	 * Uses the falloff exponent instead of inverse squared falloff when enabled.
	 * Will also change intensity units to Unitless.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (InlineEditConditionToggle, DisplayPriority = 3))
	bool bManualFalloff;

	/**
	 * Falloff exponent used when manual falloff is enabled.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (EditCondition = bManualFalloff, UIMin = 2.0f, UIMax = 16.0f, DisplayPriority = 3))
	float FalloffExponent;

	FSpotLightSettings()
		: InnerConeAngle(0.0f)
		, OuterConeAngle(44.0f)
		, SourceRadius(0.0f)
		, SoftSourceRadius(0.0f)
		, SourceLength(0.0f)
		, bManualFalloff(false)
		, FalloffExponent(8.0f)
	{}

	virtual void UpdateSettings() override;
	virtual void FromLightComponent(const ULightComponent* Target) override;
	virtual void ToLightComponent(ULightComponent* Target) const override;
};

/**
 * Stores the shared and rect light specific settings of a rect light.
 */
USTRUCT(BlueprintType)
struct TORORUNTIME_API FRectLightSettings final : public FBaseLightSettings
{
	GENERATED_BODY()

	/**
	 * Width of the rectangular light source.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (ClampMin = 10.0f, UIMin = 10.0f, DisplayPriority = 1))
	float SourceWidth;

	/**
	 * Height of the rectangular light source.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (ClampMin = 10.0f, UIMin = 10.0f, DisplayPriority = 1))
	float SourceHeight;

	/**
	 * Angle of the barn doors that shape the light.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (ClampMin = 0.0f, ClampMax = 90.0f, UIMin = 0.0f, UIMax = 90.0f, DisplayPriority = 1))
	float BarnDoorAngle;

	/**
	 * Length of the barn doors that shape the light.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (ClampMin = 1.0f, UIMin = 1.0f, DisplayPriority = 1))
	float BarnDoorLength;

	/**
	 * Optional texture projected by the rect light. Lazily loaded using soft-ptr.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (DisplayPriority = 1))
	TSoftObjectPtr<UTexture> SourceTexture;

	FRectLightSettings()
		: SourceWidth(64.0f)
		, SourceHeight(64.0f)
		, BarnDoorAngle(88.0f)
		, BarnDoorLength(20.0f)
		, SourceTexture(nullptr)
	{}

	virtual void UpdateSettings() override;
	virtual void FromLightComponent(const ULightComponent* Target) override;
	virtual void ToLightComponent(ULightComponent* Target) const override;
};

/**
 * Provides Blueprint functions to read and apply the light settings structs.
 */
UCLASS(NotBlueprintable, NotBlueprintType)
class TORORUNTIME_API ULightSettingsLibrary final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Rendering|Components|Light", meta = (DefaultToSelf = Target))
	static void SetLightDrawDistanceSettings(ULightComponent* Target, const FLightDrawDistanceSettings& Settings);

	UFUNCTION(BlueprintPure, Category = "Rendering|Components|Light", meta = (DefaultToSelf = Target))
	static void GetLightDrawDistanceSettings(FLightDrawDistanceSettings& OutData, const ULightComponent* Target);

	UFUNCTION(BlueprintCallable, Category = "Rendering|Components|Light")
	static void UpdateBaseLightSettings(UPARAM(ref) FBaseLightSettings& Settings);

	UFUNCTION(BlueprintCallable, Category = "Rendering|Components|Light", meta = (DefaultToSelf = Target))
	static void SetBaseLightSettings(ULightComponent* Target, const FBaseLightSettings& Settings);

	UFUNCTION(BlueprintPure, Category = "Rendering|Components|Light", meta = (DefaultToSelf = Target))
	static void GetBaseLightSettings(FBaseLightSettings& OutData, const ULightComponent* Target);

	UFUNCTION(BlueprintCallable, Category = "Rendering|Components|Light")
	static void UpdatePointLightSettings(UPARAM(ref) FPointLightSettings& Settings);

	UFUNCTION(BlueprintCallable, Category = "Rendering|Components|Light", meta = (DefaultToSelf = Target))
	static void SetPointLightSettings(UPointLightComponent* Target, const FPointLightSettings& Settings);

	UFUNCTION(BlueprintPure, Category = "Rendering|Components|Light", meta = (DefaultToSelf = Target))
	static void GetPointLightSettings(FPointLightSettings& OutData, const UPointLightComponent* Target);

	UFUNCTION(BlueprintCallable, Category = "Rendering|Components|Light")
	static void UpdateSpotLightSettings(UPARAM(ref) FSpotLightSettings& Settings);

	UFUNCTION(BlueprintCallable, Category = "Rendering|Components|Light", meta = (DefaultToSelf = Target))
	static void SetSpotLightSettings(USpotLightComponent* Target, const FSpotLightSettings& Settings);

	UFUNCTION(BlueprintPure, Category = "Rendering|Components|Light", meta = (DefaultToSelf = Target))
	static void GetSpotLightSettings(FSpotLightSettings& OutData, const USpotLightComponent* Target);

	UFUNCTION(BlueprintCallable, Category = "Rendering|Components|Light")
	static void UpdateRectLightSettings(UPARAM(ref) FRectLightSettings& Settings);

	UFUNCTION(BlueprintCallable, Category = "Rendering|Components|Light", meta = (DefaultToSelf = Target))
	static void SetRectLightSettings(URectLightComponent* Target, const FRectLightSettings& Settings);

	UFUNCTION(BlueprintPure, Category = "Rendering|Components|Light", meta = (DefaultToSelf = Target))
	static void GetRectLightSettings(FRectLightSettings& OutData, const URectLightComponent* Target);
};
