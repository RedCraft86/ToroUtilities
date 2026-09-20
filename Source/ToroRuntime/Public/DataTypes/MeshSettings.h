// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Components/SplineMeshComponent.h"
#include "MeshSettings.generated.h"

/**
 * Captures a static mesh component's mesh, materials, shadows, and optional world transform.
 */
USTRUCT(BlueprintType)
struct TORORUNTIME_API FStaticMeshSettings
{
	GENERATED_BODY()

	/**
	 * Mesh asset assigned to the component. Lazily loaded using soft-ptr.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
		TSoftObjectPtr<UStaticMesh> StaticMesh;

	/**
	 * Materials by slot index. Empty slots are filled from mesh defaults by FillEmptyMaterials(). All lazily loaded using soft-ptr.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
		TArray<TSoftObjectPtr<UMaterialInterface>> Materials;

	/**
	 * Optional overlay material. Lazily loaded using soft-ptr.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
		TSoftObjectPtr<UMaterialInterface> OverlayMaterial;

	/**
	 * Whether the component casts shadows.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
		bool bCastShadows;

	/**
	 * Whether applying these settings also changes the component's world transform.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (InlineEditConditionToggle))
		bool bUseTransform;

	/**
	 * World transform used when bUseTransform is enabled.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (EditCondition = bUseTransform, AllowPreserveRatio = true))
		FTransform Transform;

	FStaticMeshSettings()
		: StaticMesh(nullptr), Materials({}), bCastShadows(true), bUseTransform(false)
	{}

	virtual ~FStaticMeshSettings() = default;

	bool operator==(const FStaticMeshSettings& Other) const { return Equals(Other, false); }
	bool operator!=(const FStaticMeshSettings& Other) const { return !Equals(Other, false); }

	/**
	 * Compares mesh, materials, overlay, shadows, and optionally the transform and its enable flag.
	 * @param Other Settings to compare against.
	 * @param bCheckTransform Whether to include the optional transform in the comparison.
	 * @return True if the selected mesh settings match.
	 */
	virtual bool Equals(const FStaticMeshSettings& Other, const bool bCheckTransform) const;

	/**
	 * Whether a mesh asset is assigned, without loading it.
	 */
	FORCEINLINE bool IsValid() const
	{
		return !StaticMesh.IsNull();
	}

	/**
	 * Fills empty material slots from the mesh defaults, or clears them if the mesh cannot be loaded.
	 */
	void FillEmptyMaterials();

	/**
	 * Reads settings from Target and optionally captures its world transform. A null target leaves the settings unchanged.
	 * @param Target Component to read.
	 * @param bIncludeTransform Whether to copy the component transform.
	 */
	virtual void FromMeshComponent(const UStaticMeshComponent* Target, const bool bIncludeTransform = false);

	/**
	 * Applies mesh, material, shadow, and optionally world transform settings to Target.
	 * @param Target Component to update.
	 */
	virtual void ToMeshComponent(UStaticMeshComponent* Target) const;

	FORCEINLINE friend uint32 GetTypeHash(const FStaticMeshSettings& InMeshSettings)
	{
		uint32 Hash = GetTypeHash(InMeshSettings.StaticMesh);
		Hash = HashCombine(Hash, GetTypeHash(InMeshSettings.bCastShadows));
		Hash = HashCombine(Hash, GetTypeHash(InMeshSettings.OverlayMaterial));
		return HashCombine(Hash, GetTypeHash(InMeshSettings.Materials));
	}
};

/**
 * Adds the forward axis of a spline mesh to the static mesh settings.
 */
USTRUCT(BlueprintType)
struct TORORUNTIME_API FSplineMeshSettings final : public FStaticMeshSettings
{
	GENERATED_BODY()

	/**
	 * Axis along which the spline mesh is oriented.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MeshSettings)
		TEnumAsByte<ESplineMeshAxis::Type> ForwardAxis;

	FSplineMeshSettings()
		: ForwardAxis(ESplineMeshAxis::X)
	{}

	FORCEINLINE explicit operator FStaticMeshSettings&() { return *this; }
	FORCEINLINE explicit operator const FStaticMeshSettings&() const { return *this; }

	bool operator==(const FSplineMeshSettings& Other) const { return Equals(Other, false); }
	bool operator!=(const FSplineMeshSettings& Other) const { return !Equals(Other, false); }

	/**
	 * Compares the forward axis and inherited settings, optionally including the transform.
	 * @param Other Settings to compare against.
	 * @param bCheckTransform Whether to include the optional transform in the comparison.
	 * @return True if the forward axis and selected inherited settings match.
	 */
	bool Equals(const FSplineMeshSettings& Other, const bool bCheckTransform) const;

	/**
	 * Reads the forward axis and inherited settings from Target.
	 * @param Target Component to read.
	 * @param bIncludeTransform Whether to copy the component transform.
	 */
	virtual void FromMeshComponent(const UStaticMeshComponent* Target, const bool bIncludeTransform = false) override;

	/**
	 * Applies inherited settings and the forward axis to Target.
	 * @param Target Component to update.
	 */
	virtual void ToMeshComponent(UStaticMeshComponent* Target) const override;

	FORCEINLINE friend uint32 GetTypeHash(const FSplineMeshSettings& InSettings)
	{
		return HashCombine(GetTypeHash(InSettings.ForwardAxis),
			GetTypeHash(static_cast<FStaticMeshSettings>(InSettings))
		);
	}

private:

	/**
	 * Prevent external code from calling this base type overload
	 */
	virtual bool Equals(const FStaticMeshSettings& Other, const bool bCheckTransform = false) const override
	{
		return Super::Equals(Other, bCheckTransform);
	}
};

/**
 * Exposes static and spline mesh settings to Blueprints.
 */
UCLASS(NotBlueprintable, NotBlueprintType)
class TORORUNTIME_API UMeshSettingsLibrary final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/**
	 * Returns whether the settings reference a mesh asset.
	 * @param Settings Settings to read or update.
	 * @return whether the settings reference a mesh asset.
	 */
	UFUNCTION(BlueprintPure, Category = "StaticMesh|MeshSettings", DisplayName = "Is Valid")
		static bool IsValidStaticMeshSettings(const FStaticMeshSettings& Settings);

	/**
	 * Compares settings, optionally including the transform.
	 * @param A First settings value to compare.
	 * @param B Second settings value to compare.
	 * @param bCheckTransform Whether to include the optional transform in the comparison.
	 * @return True if the selected settings match.
	 */
	UFUNCTION(BlueprintPure, Category = "StaticMesh|MeshSettings", DisplayName = "Is Equal")
		static bool IsStaticMeshSettingsEqual(const FStaticMeshSettings& A, const FStaticMeshSettings& B, const bool bCheckTransform);

	/**
	 * Fills empty material slots from the mesh defaults.
	 * @param Settings Settings to read or update.
	 */
	UFUNCTION(BlueprintCallable, Category = "StaticMesh|MeshSettings", DisplayName = "Fill Empty Materials")
		static void FillEmptyStaticMeshMaterials(UPARAM(ref) FStaticMeshSettings& Settings);

	/**
	 * Reads settings from the target component into OutData.
	 * @param OutData Receives the settings read from the component.
	 * @param Target Component to read.
	 * @param bIncludeTransform Whether to copy the component transform.
	 */
	UFUNCTION(BlueprintCallable, Category = "StaticMesh|MeshSettings", DisplayName = "Get Static Mesh Settings")
		static void GetStaticMeshSettings(FStaticMeshSettings& OutData, const UStaticMeshComponent* Target, const bool bIncludeTransform);

	/**
	 * Applies settings to the target component.
	 * @param Target Component to update.
	 * @param Settings Settings to apply to the component.
	 */
	UFUNCTION(BlueprintCallable, Category = "StaticMesh|MeshSettings", DisplayName = "Set Static Mesh Settings")
		static void SetStaticMeshSettings(UStaticMeshComponent* Target, UPARAM(ref) FStaticMeshSettings& Settings);

	/**
	 * Returns whether the settings reference a mesh asset.
	 * @param Settings Settings to read or update.
	 * @return whether the settings reference a mesh asset.
	 */
	UFUNCTION(BlueprintPure, Category = "StaticMesh|MeshSettings", DisplayName = "Is Valid")
		static bool IsValidSplineMeshSettings(const FSplineMeshSettings& Settings);

	/**
	 * Compares settings, optionally including the transform.
	 * @param A First settings value to compare.
	 * @param B Second settings value to compare.
	 * @param bCheckTransform Whether to include the optional transform in the comparison.
	 * @return True if the selected settings match.
	 */
	UFUNCTION(BlueprintPure, Category = "StaticMesh|MeshSettings", DisplayName = "Is Equal")
		static bool IsSplineMeshSettingsEqual(const FSplineMeshSettings& A, const FSplineMeshSettings& B, const bool bCheckTransform);

	/**
	 * Fills empty material slots from the mesh defaults.
	 * @param Settings Settings to read or update.
	 */
	UFUNCTION(BlueprintCallable, Category = "StaticMesh|MeshSettings", DisplayName = "Fill Empty Materials")
		static void FillEmptySplineMeshMaterials(UPARAM(ref) FSplineMeshSettings& Settings);

	/**
	 * Reads settings from the target component into OutData.
	 * @param OutData Receives the settings read from the component.
	 * @param Target Component to read.
	 * @param bIncludeTransform Whether to copy the component transform.
	 */
	UFUNCTION(BlueprintCallable, Category = "StaticMesh|MeshSettings", DisplayName = "Get Spline Mesh Settings")
		static void GetSplineMeshSettings(FSplineMeshSettings& OutData, const USplineMeshComponent* Target, const bool bIncludeTransform);

	/**
	 * Applies settings to the target component.
	 * @param Target Component to update.
	 * @param Settings Settings to apply to the component.
	 */
	UFUNCTION(BlueprintCallable, Category = "StaticMesh|MeshSettings", DisplayName = "Set Spline Mesh Settings")
		static void SetSplineMeshSettings(USplineMeshComponent* Target, UPARAM(ref) FSplineMeshSettings& Settings);
};