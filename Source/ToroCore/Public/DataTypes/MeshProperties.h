// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "Components/SplineMeshComponent.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MeshProperties.generated.h"

/**
 * A serializable data structure representing the state of a Static Mesh Component.
 * This struct facilitates the synchronization, storage, and replication of mesh data.
 * 
 * Note: Uses TSoftObjectPtr for asset references to prevent "hard" dependencies, 
 * allowing for efficient memory management and asynchronous loading patterns.
 */
USTRUCT(BlueprintType)
struct TOROCORE_API FStaticMeshProperties
{
	GENERATED_BODY()

	/** The Static Mesh asset to be applied to the component. */
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MeshProperties)
		TSoftObjectPtr<UMaterialInterface> OverlayMaterial;

	/** Whether the mesh should cast dynamic shadows. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MeshProperties)
		bool bCastShadows;

	/** Toggle to determine if the Transform property below should be applied during component synchronization. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MeshProperties, meta = (InlineEditConditionToggle))
		bool bUseTransform;

	/** 
	 * The relative transform (Location, Rotation, Scale) for the mesh. 
	 * Only applied if bUseTransform is true.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MeshProperties, meta = (EditCondition = "bUseTransform", AllowPreserveRatio = true))
		FTransform Transform;

	FStaticMeshProperties(): StaticMesh(nullptr), Materials({}), bCastShadows(true), bUseTransform(false) {}
	virtual ~FStaticMeshProperties() = default;

	FORCEINLINE bool IsValid() const { return !StaticMesh.IsNull(); }

	void FillEmptyMaterials();
	virtual void FromMeshComponent(const UStaticMeshComponent* Target, const bool bIncludeTransform = false);
	virtual void ToMeshComponent(UStaticMeshComponent* Target);

	bool EqualsTo(const FStaticMeshProperties& Other, const bool bCheckTransform) const;
	FORCEINLINE bool operator==(const FStaticMeshProperties& Other) const { return EqualsTo(Other, false); }
	FORCEINLINE bool operator!=(const FStaticMeshProperties& Other) const { return !EqualsTo(Other, false); }

	inline friend uint32 GetTypeHash(const FStaticMeshProperties& InMeshProperties)
	{
		uint32 Hash = GetTypeHash(InMeshProperties.StaticMesh);
		Hash = HashCombine(Hash, GetTypeHash(InMeshProperties.bCastShadows));
		Hash = HashCombine(Hash, GetTypeHash(InMeshProperties.OverlayMaterial));
		return HashCombine(Hash, GetTypeHash(InMeshProperties.Materials));
	}
};

/**
 * Specialized properties for Spline Mesh Components, extending the standard Static Mesh 
 * properties with spline-specific configuration such as the Forward Axis.
 */
USTRUCT(BlueprintType)
struct TOROCORE_API FSplineMeshProperties final : public FStaticMeshProperties
{
	GENERATED_BODY()

	/** The axis of the mesh that should be aligned along the spline direction. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MeshProperties)
		TEnumAsByte<ESplineMeshAxis::Type> ForwardAxis;

    FSplineMeshProperties(): ForwardAxis(ESplineMeshAxis::X) {}

	virtual void FromMeshComponent(const UStaticMeshComponent* Target, const bool bIncludeTransform = false) override;
	virtual void ToMeshComponent(UStaticMeshComponent* Target) override;

	bool EqualsTo(const FSplineMeshProperties& Other, const bool bCheckTransform) const;
	FORCEINLINE bool operator==(const FSplineMeshProperties& Other) const { return EqualsTo(Other, false); }
	FORCEINLINE bool operator!=(const FSplineMeshProperties& Other) const { return !EqualsTo(Other, false); }
	FORCEINLINE const FStaticMeshProperties& operator*() const { return *this; }

	inline friend uint32 GetTypeHash(const FSplineMeshProperties& InMeshProperties)
	{
		return HashCombine(GetTypeHash(*InMeshProperties), GetTypeHash(InMeshProperties.ForwardAxis));
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
	 * @param MeshProperties The property struct to validate.
	 * @return True if the StaticMesh pointer is not null.
	 */
	UFUNCTION(BlueprintPure, Category = "StaticMesh|MeshProperties", DisplayName = "Is Valid")
	static bool IsValidStaticMeshProp(const FStaticMeshProperties& MeshProperties)
	{
		return MeshProperties.IsValid();
	}

	/** 
	 * Synchronizes the internal material array size with the number of slots on the assigned Static Mesh.
	 * @param MeshProperties The property struct to update (Pass-by-Reference).
	 */
	UFUNCTION(BlueprintCallable, Category = "StaticMesh|MeshProperties", DisplayName = "Fill Empty Materials")
	static void FillEmptyStaticMeshMaterials(UPARAM(ref) FStaticMeshProperties& MeshProperties)
	{
		MeshProperties.FillEmptyMaterials();
	}

	/** 
	 * Populates a property struct using data from an existing Static Mesh Component.
	 * @param OutData The struct to receive the extracted data.
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
	 * @param MeshProperties The property struct containing the desired state.
	 */
	UFUNCTION(BlueprintCallable, Category = "StaticMesh|MeshProperties", DisplayName = "Set Static Mesh Properties")
	static void SetStaticMeshProp(UStaticMeshComponent* Target, UPARAM(ref) FStaticMeshProperties& MeshProperties)
	{
		MeshProperties.ToMeshComponent(Target);
	}

	
	/** 
	 * Checks if the provided Spline Mesh properties contain a valid mesh asset.
	 * @param MeshProperties The property struct to validate.
	 * @return True if the StaticMesh pointer is not null.
	 */
	UFUNCTION(BlueprintPure, Category = "StaticMesh|MeshProperties", DisplayName = "Is Valid")
	static bool IsValidSplineMeshProp(const FSplineMeshProperties& MeshProperties)
	{
		return MeshProperties.IsValid();
	}

	/** 
	 * Synchronizes the internal material array size with the number of slots on the assigned Static Mesh.
	 * @param MeshProperties The property struct to update (Pass-by-Reference).
	 */
	UFUNCTION(BlueprintCallable, Category = "StaticMesh|MeshProperties", DisplayName = "Fill Empty Materials")
	static void FillEmptySplineMeshMaterials(UPARAM(ref) FSplineMeshProperties& MeshProperties)
	{
		MeshProperties.FillEmptyMaterials();
	}

	/** 
	 * Populates a property struct using data from an existing Spline Mesh Component.
	 * @param OutData The struct to receive the extracted data.
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
	 * @param MeshProperties The property struct containing the desired state.
	 */
	UFUNCTION(BlueprintCallable, Category = "StaticMesh|MeshProperties", DisplayName = "Set Spline Mesh Properties")
	static void SetSplineMeshProp(USplineMeshComponent* Target, UPARAM(ref) FSplineMeshProperties& MeshProperties)
	{
		MeshProperties.ToMeshComponent(Target);
	}
};