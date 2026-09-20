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

	virtual bool Equals(const FStaticMeshSettings& Other, const bool bCheckTransform) const;
	bool operator==(const FStaticMeshSettings& Other) const { return Equals(Other, false); }
	bool operator!=(const FStaticMeshSettings& Other) const { return !Equals(Other, false); }

	FORCEINLINE bool IsValid() const
	{
		return !StaticMesh.IsNull();
	}

	void FillEmptyMaterials();
	virtual void FromMeshComponent(const UStaticMeshComponent* Target, const bool bIncludeTransform = false);
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

	bool Equals(const FSplineMeshSettings& Other, const bool bCheckTransform) const;
	bool operator==(const FSplineMeshSettings& Other) const { return Equals(Other, false); }
	bool operator!=(const FSplineMeshSettings& Other) const { return !Equals(Other, false); }

	virtual void FromMeshComponent(const UStaticMeshComponent* Target, const bool bIncludeTransform = false) override;
	virtual void ToMeshComponent(UStaticMeshComponent* Target) const override;

	FORCEINLINE friend uint32 GetTypeHash(const FSplineMeshSettings& InSettings)
	{
		return HashCombine(GetTypeHash(InSettings.ForwardAxis),
			GetTypeHash(static_cast<FStaticMeshSettings>(InSettings))
		);
	}

private:

	// Prevent external code from calling this base type overload
	virtual bool Equals(const FStaticMeshSettings& Other, const bool bCheckTransform = false) const override
	{
		return Super::Equals(Other, bCheckTransform);
	}
};

/**
 * Exposes mesh settings comparison and component transfer to Blueprints.
 */
UCLASS(NotBlueprintable, NotBlueprintType)
class TORORUNTIME_API UMeshSettingsLibrary final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, Category = "StaticMesh|MeshSettings", DisplayName = "Is Valid")
		static bool IsValidStaticMeshSettings(const FStaticMeshSettings& Settings);

	UFUNCTION(BlueprintPure, Category = "StaticMesh|MeshSettings", DisplayName = "Is Equal")
		static bool IsStaticMeshSettingsEqual(const FStaticMeshSettings& A, const FStaticMeshSettings& B, const bool bCheckTransform);

	UFUNCTION(BlueprintCallable, Category = "StaticMesh|MeshSettings", DisplayName = "Fill Empty Materials")
		static void FillEmptyStaticMeshMaterials(UPARAM(ref) FStaticMeshSettings& Settings);

	UFUNCTION(BlueprintCallable, Category = "StaticMesh|MeshSettings", DisplayName = "Get Static Mesh Settings")
		static void GetStaticMeshSettings(FStaticMeshSettings& OutData, const UStaticMeshComponent* Target, const bool bIncludeTransform);

	UFUNCTION(BlueprintCallable, Category = "StaticMesh|MeshSettings", DisplayName = "Set Static Mesh Settings")
		static void SetStaticMeshSettings(UStaticMeshComponent* Target, UPARAM(ref) FStaticMeshSettings& Settings);

	UFUNCTION(BlueprintPure, Category = "StaticMesh|MeshSettings", DisplayName = "Is Valid")
		static bool IsValidSplineMeshSettings(const FSplineMeshSettings& Settings);

	UFUNCTION(BlueprintPure, Category = "StaticMesh|MeshSettings", DisplayName = "Is Equal")
		static bool IsSplineMeshSettingsEqual(const FSplineMeshSettings& A, const FSplineMeshSettings& B, const bool bCheckTransform);

	UFUNCTION(BlueprintCallable, Category = "StaticMesh|MeshSettings", DisplayName = "Fill Empty Materials")
		static void FillEmptySplineMeshMaterials(UPARAM(ref) FSplineMeshSettings& Settings);

	UFUNCTION(BlueprintCallable, Category = "StaticMesh|MeshSettings", DisplayName = "Get Spline Mesh Settings")
		static void GetSplineMeshSettings(FSplineMeshSettings& OutData, const USplineMeshComponent* Target, const bool bIncludeTransform);

	UFUNCTION(BlueprintCallable, Category = "StaticMesh|MeshSettings", DisplayName = "Set Spline Mesh Settings")
		static void SetSplineMeshSettings(USplineMeshComponent* Target, UPARAM(ref) FSplineMeshSettings& Settings);
};