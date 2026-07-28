// Copyright (C) 2026 Tayzar Linn. Licensed under LGPL-3.0, see project LICENSE file.

#pragma once

#include "Engine/StaticMesh.h"
#include "UObject/SoftObjectPtr.h"
#include "Components/SplineMeshComponent.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MeshProperties.generated.h"

/**
 * A struct representing basic properties in a Static Mesh Component.
 */
USTRUCT(BlueprintType)
struct TOROCORE_API FStaticMeshProperties
{
	GENERATED_BODY()

	/** 
	 * The Static Mesh asset to be applied to the component. 
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MeshProperties)
		TSoftObjectPtr<UStaticMesh> StaticMesh;

	/** 
	 * Array of materials used to override the default materials on the mesh. 
	 * Elements correspond to the material slots of the Static Mesh.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MeshProperties)
		TArray<TSoftObjectPtr<UMaterialInterface>> Materials;

	/** 
	 * Optional overlay material applied to the entire mesh (e.g., for selection highlights or status effects). 
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
		TSoftObjectPtr<UMaterialInterface> OverlayMaterial;

	/** 
	 * Whether the mesh should cast shadows. 
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
		bool bCastShadows;

	/** 
	 * If the Transform property should be applied during "ToMeshComponent" calls. 
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties, meta = (InlineEditConditionToggle))
		bool bUseTransform;

	/** 
	 * The relative transform (Location, Rotation, Scale) for the mesh. 
	 * Only applied if bUseTransform is true.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties, meta = (EditCondition = "bUseTransform", AllowPreserveRatio = true))
		FTransform Transform;

	FStaticMeshProperties(): StaticMesh(nullptr), Materials({}), bCastShadows(true), bUseTransform(false) {}
	virtual ~FStaticMeshProperties() = default;

	FORCEINLINE bool IsValid() const
	{
		return !StaticMesh.IsNull();
	}

	void FillEmptyMaterials();
	virtual bool FromMeshComponent(const UStaticMeshComponent* Target, const bool bIncludeTransform = false);
	virtual bool ToMeshComponent(UStaticMeshComponent* Target) const;

	virtual bool Equals(const FStaticMeshProperties& Other, const bool bCheckTransform) const;
	bool operator==(const FStaticMeshProperties& Other) const { return Equals(Other, false); }
	bool operator!=(const FStaticMeshProperties& Other) const { return !Equals(Other, false); }

	FORCEINLINE friend uint32 GetTypeHash(const FStaticMeshProperties& InMeshProperties)
	{
		uint32 Hash = GetTypeHash(InMeshProperties.StaticMesh);
		Hash = HashCombine(Hash, GetTypeHash(InMeshProperties.bCastShadows));
		Hash = HashCombine(Hash, GetTypeHash(InMeshProperties.OverlayMaterial));
		return HashCombine(Hash, GetTypeHash(InMeshProperties.Materials));
	}
};

/**
 * A struct representing basic properties in a Spline Mesh Component.
 */
USTRUCT(BlueprintType)
struct TOROCORE_API FSplineMeshProperties final : public FStaticMeshProperties
{
	GENERATED_BODY()

	/** 
	 * The axis of the mesh that should be aligned along the spline direction. 
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MeshProperties)
		TEnumAsByte<ESplineMeshAxis::Type> ForwardAxis;

	FSplineMeshProperties(): ForwardAxis(ESplineMeshAxis::X) {}

	virtual bool FromMeshComponent(const UStaticMeshComponent* Target, const bool bIncludeTransform = false) override;
	virtual bool ToMeshComponent(UStaticMeshComponent* Target) const override;

	bool Equals(const FSplineMeshProperties& Other, const bool bCheckTransform) const;
	bool operator==(const FSplineMeshProperties& Other) const { return Equals(Other, false); }
	bool operator!=(const FSplineMeshProperties& Other) const { return !Equals(Other, false); }

	FORCEINLINE explicit operator FStaticMeshProperties&() { return *this; }
	FORCEINLINE explicit operator const FStaticMeshProperties&() const { return *this; }

	FORCEINLINE friend uint32 GetTypeHash(const FSplineMeshProperties& InProperties)
	{
		return HashCombine(GetTypeHash(InProperties.ForwardAxis),
			GetTypeHash(static_cast<FStaticMeshProperties>(InProperties))
		);
	}

private:

	// Prevent external code from calling this base type
	virtual bool Equals(const FStaticMeshProperties& Other, const bool bCheckTransform = false) const override
	{
		return Super::Equals(Other, bCheckTransform);
	}
};

/**
 * Blueprint function wrappers for MeshProperties struct functions
 */
UCLASS(NotBlueprintable, NotBlueprintType)
class TOROCORE_API UMeshPropertiesLibrary final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/** 
	 * Checks if the provided Static Mesh properties contain a valid mesh asset.
	 * @param MeshProperties The properties to check.
	 * @return True if the StaticMesh pointer is not null.
	 */
	UFUNCTION(BlueprintPure, Category = "StaticMesh|MeshProperties", DisplayName = "Is Valid")
	static bool IsValidStaticMeshProp(const FStaticMeshProperties& MeshProperties)
	{
		return MeshProperties.IsValid();
	}

	/** 
	 * Synchronizes the internal material array size with the number of slots on the assigned Static Mesh.
	 * @param MeshProperties The properties to modify.
	 */
	UFUNCTION(BlueprintCallable, Category = "StaticMesh|MeshProperties", DisplayName = "Fill Empty Materials")
	static void FillEmptyStaticMeshMaterials(UPARAM(ref) FStaticMeshProperties& MeshProperties)
	{
		MeshProperties.FillEmptyMaterials();
	}

	/** 
	 * Populates a property struct using data from an existing Static Mesh Component.
	 * @param OutData Properties extracted from the mesh.
	 * @param Target The source component to read from.
	 * @param bIncludeTransform If true, the component's relative transform is captured.
	 */
	UFUNCTION(BlueprintCallable, Category = "StaticMesh|MeshProperties", DisplayName = "Get Static Mesh Properties")
	static void GetStaticMeshProp(FStaticMeshProperties& OutData, const UStaticMeshComponent* Target, const bool bIncludeTransform)
	{
		OutData.FromMeshComponent(Target, bIncludeTransform);
	}

	/** 
	 * Applies the data stored in a property struct to a Static Mesh Component.
	 * @param Target The component to update.
	 * @param MeshProperties The properties to use.
	 */
	UFUNCTION(BlueprintCallable, Category = "StaticMesh|MeshProperties", DisplayName = "Set Static Mesh Properties")
	static void SetStaticMeshProp(UStaticMeshComponent* Target, UPARAM(ref) FStaticMeshProperties& MeshProperties)
	{
		MeshProperties.ToMeshComponent(Target);
	}

	/** 
	 * Checks if the provided Spline Mesh properties contain a valid mesh asset.
	 * @param MeshProperties The properties to check.
	 * @return True if the StaticMesh pointer is not null.
	 */
	UFUNCTION(BlueprintPure, Category = "StaticMesh|MeshProperties", DisplayName = "Is Valid")
	static bool IsValidSplineMeshProp(const FSplineMeshProperties& MeshProperties)
	{
		return MeshProperties.IsValid();
	}

	/** 
	 * Synchronizes the internal material array size with the number of slots on the assigned Static Mesh.
	 * @param MeshProperties The properties to modify.
	 */
	UFUNCTION(BlueprintCallable, Category = "StaticMesh|MeshProperties", DisplayName = "Fill Empty Materials")
	static void FillEmptySplineMeshMaterials(UPARAM(ref) FSplineMeshProperties& MeshProperties)
	{
		MeshProperties.FillEmptyMaterials();
	}

	/** 
	 * Populates a property struct using data from an existing Spline Mesh Component.
	 * @param OutData Properties extracted from the mesh.
	 * @param Target The source component to read from.
	 * @param bIncludeTransform If true, the component's relative transform is captured.
	 */
	UFUNCTION(BlueprintCallable, Category = "StaticMesh|MeshProperties", DisplayName = "Get Spline Mesh Properties")
	static void GetSplineMeshProp(FSplineMeshProperties& OutData, const USplineMeshComponent* Target, const bool bIncludeTransform)
	{
		OutData.FromMeshComponent(Target, bIncludeTransform);
	}

	/** 
	 * Applies the data stored in a property struct to a Spline Mesh Component.
	 * @param Target The component to update.
	 * @param MeshProperties The properties to use.
	 */
	UFUNCTION(BlueprintCallable, Category = "StaticMesh|MeshProperties", DisplayName = "Set Spline Mesh Properties")
	static void SetSplineMeshProp(USplineMeshComponent* Target, UPARAM(ref) FSplineMeshProperties& MeshProperties)
	{
		MeshProperties.ToMeshComponent(Target);
	}
};
