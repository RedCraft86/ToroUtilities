// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "DataTypes/LightingData.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ToroLightingUtils.generated.h"

class ULightComponent;
class URectLightComponent;
class USpotLightComponent;
class UPointLightComponent;

UCLASS(DisplayName = "Lighting Utils")
class TOROCORE_API UToroLightingUtils final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

    UFUNCTION(BlueprintPure, Category = "Rendering|Components|Light|Properties", DisplayName = "Base Light Properties (From Point)", meta = (BlueprintAutocast = true, CompactNodeTitle = "->"))
        static FBaseLightProperties GetBaseProperties_Point(const FPointLightProperties& Target);

    UFUNCTION(BlueprintPure, Category = "Rendering|Components|Light|Properties", DisplayName = "Base Light Properties (From Spot)", meta = (BlueprintAutocast = true, CompactNodeTitle = "->"))
        static FBaseLightProperties GetBaseProperties_Spot(const FSpotLightProperties& Target);

    UFUNCTION(BlueprintPure, Category = "Rendering|Components|Light|Properties", DisplayName = "Base Light Properties (From Rect)", meta = (BlueprintAutocast = true, CompactNodeTitle = "->"))
        static FBaseLightProperties GetBaseProperties_Rect(const FRectLightProperties& Target);

    UFUNCTION(BlueprintPure, Category = "Rendering|Components|Light|Properties", DisplayName = "Point Light Properties (From Base)", meta = (BlueprintAutocast = true, CompactNodeTitle = "->"))
        static FPointLightProperties GetPointProperties(const FBaseLightProperties& Target, const bool bBasicData = false);

    UFUNCTION(BlueprintPure, Category = "Rendering|Components|Light|Properties", DisplayName = "Spot Light Properties (From Base)", meta = (BlueprintAutocast = true, CompactNodeTitle = "->"))
        static FSpotLightProperties GetSpotProperties(const FBaseLightProperties& Target, const bool bBasicData = false);

    UFUNCTION(BlueprintPure, Category = "Rendering|Components|Light|Properties", DisplayName = "Rect Light Properties (From Base)", meta = (BlueprintAutocast = true, CompactNodeTitle = "->"))
        static FRectLightProperties GetRectProperties(const FBaseLightProperties& Target, const bool bBasicData = false);
	
	UFUNCTION(BlueprintCallable, Category = "Rendering|Components|Light", meta = (DefaultToSelf = "Target"))
		static void SetLightDrawDistanceSettings(ULightComponent* Target, const FLightDrawDistance& Settings);

	UFUNCTION(BlueprintPure, Category = "Rendering|Components|Light", meta = (DefaultToSelf = "Target"))
		static FLightDrawDistance GetLightDrawDistanceSettings(const ULightComponent* Target);

	UFUNCTION(BlueprintCallable, Category = "Rendering|Components|Light", meta = (DefaultToSelf = "Target"))
		static void SetBaseLightProperties(ULightComponent* Target, const FBaseLightProperties& Properties);

	UFUNCTION(BlueprintPure, Category = "Rendering|Components|Light", meta = (DefaultToSelf = "Target"))
		static FBaseLightProperties GetBaseLightProperties(const ULightComponent* Target);

	UFUNCTION(BlueprintCallable, Category = "Rendering|Components|Light", meta = (DefaultToSelf = "Target"))
		static void SetPointLightProperties(UPointLightComponent* Target, const FPointLightProperties& Properties);

	UFUNCTION(BlueprintPure, Category = "Rendering|Components|Light", meta = (DefaultToSelf = "Target"))
		static FPointLightProperties GetPointLightProperties(const UPointLightComponent* Target);

	UFUNCTION(BlueprintCallable, Category = "Rendering|Components|Light", meta = (DefaultToSelf = "Target"))
		static void SetSpotLightProperties(USpotLightComponent* Target, const FSpotLightProperties& Properties);

	UFUNCTION(BlueprintPure, Category = "Rendering|Components|Light", meta = (DefaultToSelf = "Target"))
		static FSpotLightProperties GetSpotLightProperties(const USpotLightComponent* Target);

	UFUNCTION(BlueprintCallable, Category = "Rendering|Components|Light", meta = (DefaultToSelf = "Target"))
		static void SetRectLightProperties(URectLightComponent* Target, const FRectLightProperties& Properties);

	UFUNCTION(BlueprintPure, Category = "Rendering|Components|Light", meta = (DefaultToSelf = "Target"))
		static FRectLightProperties GetRectLightProperties(const URectLightComponent* Target);
};
