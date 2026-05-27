// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "Components/SplineMeshComponent.h"
#include "MeshProperties.generated.h"

USTRUCT(BlueprintType)
struct TOROCORE_API FStaticMeshProperties
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MeshProperties)
		TSoftObjectPtr<UStaticMesh> StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MeshProperties)
		TArray<TSoftObjectPtr<UMaterialInterface>> Materials;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MeshProperties)
		TSoftObjectPtr<UMaterialInterface> OverlayMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MeshProperties)
		bool bCastShadows;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MeshProperties, meta = (InlineEditConditionToggle))
		bool bUseTransform;

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

	inline friend uint32 GetTypeHash(const FStaticMeshProperties& InMeshData)
	{
		uint32 Hash = GetTypeHash(InMeshData.StaticMesh);
		Hash = HashCombine(Hash, GetTypeHash(InMeshData.bCastShadows));
		Hash = HashCombine(Hash, GetTypeHash(InMeshData.OverlayMaterial));
		return HashCombine(Hash, GetTypeHash(InMeshData.Materials));
	}
};

USTRUCT(BlueprintType)
struct TOROCORE_API FSplineMeshProperties final : public FStaticMeshProperties
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MeshProperties)
		TEnumAsByte<ESplineMeshAxis::Type> ForwardAxis;

    FSplineMeshProperties(): ForwardAxis(ESplineMeshAxis::X) {}

	virtual void FromMeshComponent(const UStaticMeshComponent* Target, const bool bIncludeTransform = false) override;
	virtual void ToMeshComponent(UStaticMeshComponent* Target) override;

	bool EqualsTo(const FSplineMeshProperties& Other, const bool bCheckTransform) const;
	FORCEINLINE bool operator==(const FSplineMeshProperties& Other) const { return EqualsTo(Other, false); }
	FORCEINLINE bool operator!=(const FSplineMeshProperties& Other) const { return !EqualsTo(Other, false); }
	FORCEINLINE const FStaticMeshProperties& operator*() const { return *this; }

	inline friend uint32 GetTypeHash(const FSplineMeshProperties& InMeshData)
	{
		return HashCombine(GetTypeHash(*InMeshData), GetTypeHash(InMeshData.ForwardAxis));
	}
};
