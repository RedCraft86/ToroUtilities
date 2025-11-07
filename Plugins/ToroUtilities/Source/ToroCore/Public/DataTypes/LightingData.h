// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "LightingData.generated.h"

USTRUCT(BlueprintType)
struct TOROCORE_API FLightDrawDistance
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
		bool bUseDrawDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties, meta = (EditCondition = "bUseDrawDistance", ClampMin = 0.0f, UIMin = 0.0f))
		float MaxDrawDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties, meta = (EditCondition = "bUseDrawDistance", ClampMin = 0.0f, UIMin = 0.0f))
		float MaxDistanceFadeRange;

    FLightDrawDistance()
        : bUseDrawDistance(true)
        , MaxDrawDistance(5000.0f)
        , MaxDistanceFadeRange(1000.0f)
    {}

    float GetMaxDrawDistance() const;
    float GetMaxDistanceFadeRange() const;
    bool IsValidData() const;
};

USTRUCT(BlueprintType)
struct TOROCORE_API FBaseLightProperties
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties, meta = (DisplayPriority = 0))
        ELightUnits IntensityUnits;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties, meta = (ClampMin = 0.0f, UIMin = 0.0f, ShouldShowInViewport = true, DisplayPriority = 0))
        float Intensity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties, meta = (HideAlphaChannel, ShouldShowInViewport = true, DisplayPriority = 0))
        FLinearColor Color;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties, meta = (ClampMin = 0.0f, UIMin = 8.0f, UIMax = 16384.0f, ShouldShowInViewport = true, DisplayPriority = 0))
        float AttenuationRadius;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties, meta = (InlineEditConditionToggle, DisplayPriority = 2))
        bool bUseTemperature;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties, meta = (UIMin = 1700.0f, UIMax = 12000.0f, ShouldShowInViewport = true, EditCondition = "bUseTemperature", DisplayPriority = 2))
        float Temperature;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties, meta = (UIMin = 0.0f, UIMax = 6.0f, DisplayPriority = 2))
        float IndirectIntensity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties, meta = (UIMin = 0.25f, UIMax = 4.0f, DisplayPriority = 2))
        float VolumetricScatteringIntensity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties, meta = (DisplayPriority = 2))
        bool bLightCastShadows;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties, meta = (DisplayPriority = 2))
        bool bCastVolumetricShadows;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties, meta = (ClampMin = 0.0f, ClampMax = 1.0f, UIMin = 0.0f, UIMax = 1.0f, DisplayPriority = 3))
        float SpecularScale;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties, meta = (DisplayPriority = 3))
        FLightDrawDistance DrawDistance;

    FBaseLightProperties()
        : IntensityUnits(ELightUnits::Candelas)
        , Intensity(8.0f)
        , Color(FLinearColor::White)
        , AttenuationRadius(1000.0f)
        , bUseTemperature(false)
        , Temperature(6500.0f)
        , IndirectIntensity(1.0f)
        , VolumetricScatteringIntensity(1.0f)
        , bLightCastShadows(true)
        , bCastVolumetricShadows(false)
        , SpecularScale(1.0f)
        , DrawDistance({})
    {}

    FBaseLightProperties GetBase() const;
    void CopyFrom(const FBaseLightProperties& Properties);
    void CopyBaseData(const FBaseLightProperties& Properties);
};

USTRUCT(BlueprintType, DisplayName = "Point Light Properties")
struct TOROCORE_API FPointLightProperties : public FBaseLightProperties
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties, meta = (ShouldShowInViewport = true, DisplayPriority = 1, ClampMin = 0.0f, UIMin = 0.0f))
        float SourceRadius;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties, meta = (DisplayPriority = 1, ClampMin = 0.0f, UIMin = 0.0f))
        float SoftSourceRadius;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties, meta = (DisplayPriority = 1, ClampMin = 0.0f, UIMin = 0.0f))
        float SourceLength;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties, meta = (InlineEditConditionToggle, DisplayPriority = 3))
        bool bUseInverseSquaredFalloff;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties, meta = (UIMin = 2.0f, UIMax = 16.0f, EditCondition = "!bUseInverseSquaredFalloff", DisplayPriority = 3))
        float LightFalloffExponent;

    FPointLightProperties()
        : SourceRadius(0.0f)
        , SoftSourceRadius(0.0f)
        , SourceLength(0.0f)
        , bUseInverseSquaredFalloff(false)
        , LightFalloffExponent(8.0f)
    {}
};

USTRUCT(BlueprintType, DisplayName = "Spot Light Properties")
struct TOROCORE_API FSpotLightProperties : public FBaseLightProperties
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties, meta = (UIMin = 1.0f, UIMax = 80.0f, ShouldShowInViewport = true, DisplayPriority = 1))
        float InnerConeAngle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties, meta = (UIMin = 1.0f, UIMax = 80.0f, ShouldShowInViewport = true, DisplayPriority = 1))
        float OuterConeAngle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties, meta = (ShouldShowInViewport = true, DisplayPriority = 1, ClampMin = 0.0f, UIMin = 0.0f))
        float SourceRadius;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties, meta = (DisplayPriority = 1, ClampMin = 0.0f, UIMin = 0.0f))
        float SoftSourceRadius;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties, meta = (DisplayPriority = 1, ClampMin = 0.0f, UIMin = 0.0f))
        float SourceLength;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties, meta = (InlineEditConditionToggle, DisplayPriority = 3))
        bool bUseInverseSquaredFalloff;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties, meta = (UIMin = 2.0f, UIMax = 16.0f, EditCondition = "!bUseInverseSquaredFalloff", DisplayPriority = 3))
        float LightFalloffExponent;

    FSpotLightProperties()
        : InnerConeAngle(0.0f)
        , OuterConeAngle(44.0f)
        , SourceRadius(0.0f)
        , SoftSourceRadius(0.0f)
        , SourceLength(0.0f)
        , bUseInverseSquaredFalloff(false)
        , LightFalloffExponent(8.0f)
    {}
};

USTRUCT(BlueprintType, DisplayName = "Rect Light Properties")
struct TOROCORE_API FRectLightProperties : public FBaseLightProperties
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties, meta = (DisplayPriority = 1, ClampMin = 10.0f, UIMin = 10.0f))
        float SourceWidth;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties, meta = (DisplayPriority = 1, ClampMin = 10.0f, UIMin = 10.0f))
        float SourceHeight;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties, meta = (ClampMin = 0.0f, ClampMax = 90.0f, UIMin = 0.0f, UIMax = 90.0f, DisplayPriority = 1))
        float BarnDoorAngle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties, meta = (ClampMin = 1.0f, UIMin = 1.0f, DisplayPriority = 1))
        float BarnDoorLength;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties, meta = (DisplayPriority = 1))
        TObjectPtr<UTexture> SourceTexture;

    FRectLightProperties()
        : SourceWidth(64.0f)
        , SourceHeight(64.0f)
        , BarnDoorAngle(88.0f)
        , BarnDoorLength(20.0f)
        , SourceTexture(nullptr)
    {}
};