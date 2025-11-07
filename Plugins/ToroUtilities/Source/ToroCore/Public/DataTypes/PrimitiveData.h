// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Components/SplineMeshComponent.h"
#include "PrimitiveData.generated.h"

USTRUCT(BlueprintType)
struct TOROCORE_API FStaticMeshData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MeshData, meta = (DisplayPriority = 0))
        TSoftObjectPtr<UStaticMesh> Mesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MeshData, meta = (DisplayPriority = 0))
        TArray<TSoftObjectPtr<UMaterialInterface>> Materials;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MeshData, meta = (DisplayPriority = 0))
        bool bCastShadows;

    FStaticMeshData(): Mesh(nullptr), Materials({}), bCastShadows(true) {}
    FStaticMeshData(const TSoftObjectPtr<UStaticMesh>& InMesh, const TArray<TSoftObjectPtr<UMaterialInterface>>& InMaterials = {}, const bool bInCastShadows = true)
        : Mesh(InMesh), Materials(InMaterials), bCastShadows(bInCastShadows)
    {}
    
    FORCEINLINE friend FArchive& operator<<(FArchive& Ar, FStaticMeshData& Data)
    {
        Ar << Data.Mesh;
        Ar << Data.Materials;
        Ar << Data.bCastShadows;
        return Ar;
    }

    FORCEINLINE bool IsValidData() const { return !Mesh.IsNull(); }
    
    void FillMaterials();
    void FromStaticMeshComponent(const UStaticMeshComponent* InComponent);
    void ToStaticMeshComponent(UStaticMeshComponent* InComponent) const;
};

USTRUCT(BlueprintType)
struct TOROCORE_API FTransformMeshData : public FStaticMeshData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MeshData, meta = (DisplayPriority = 1))
        FTransform Transform;

    FTransformMeshData() : Transform(FTransform::Identity) {}
    FORCEINLINE FStaticMeshData operator*() const { return {Mesh, Materials, bCastShadows}; }
    
    FORCEINLINE friend FArchive& operator<<(FArchive& Ar, FTransformMeshData& Data)
    {
        Ar << Data.Mesh;
        Ar << Data.Materials;
        Ar << Data.bCastShadows;
        Ar << Data.Transform;
        return Ar;
    }
};

USTRUCT(BlueprintType)
struct TOROCORE_API FSplineMeshData : public FStaticMeshData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MeshData, meta = (DisplayPriority = -1))
        TEnumAsByte<ESplineMeshAxis::Type> MeshAxis;

    FSplineMeshData() : MeshAxis(ESplineMeshAxis::X) {}
    FORCEINLINE FStaticMeshData operator*() const { return {Mesh, Materials, bCastShadows}; }
    
    FORCEINLINE friend FArchive& operator<<(FArchive& Ar, FSplineMeshData& Data)
    {
        Ar << Data.Mesh;
        Ar << Data.Materials;
        Ar << Data.bCastShadows;
        Ar << Data.MeshAxis;
        return Ar;
    }
};

USTRUCT(BlueprintType, meta = (DisableSplitPin, HasNativeMake = "/Script/ToroCore.ToroPrimitiveUtils.Make_PrimitiveCollision", HasNativeBreak = "/Script/ToroCore.ToroPrimitiveUtils.Break_PrimitiveCollision"))
struct TOROCORE_API FPrimitiveCollision
{
#if WITH_EDITOR
    friend class FPrimitiveCollisionDetails;
#endif
    
    GENERATED_BODY()

    FPrimitiveCollision()
        : CollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics)
        , ObjectType(ECC_WorldStatic)
        , Responses({})
    {
        SetProfileName(UCollisionProfile::BlockAll_ProfileName);
    }
    
    FPrimitiveCollision(const FName& ProfileName)
        : CollisionProfileName(NAME_None)
    {
        SetProfileName(ProfileName);
    }
    
    FPrimitiveCollision(const TEnumAsByte<ECollisionEnabled::Type> InCollisionEnabled, const TEnumAsByte<ECollisionChannel> InObjectType, const TMap<TEnumAsByte<ECollisionChannel>, TEnumAsByte<ECollisionResponse>>& InResponses = {})
        : CollisionEnabled(InCollisionEnabled)
        , ObjectType(InObjectType)
        , Responses(InResponses)
        , CollisionProfileName(UCollisionProfile::CustomCollisionProfileName)
    {
        FillResponsesMap();
    }

    FORCEINLINE friend FArchive& operator<<(FArchive& Ar, FPrimitiveCollision& Data)
    {
        Ar << Data.CollisionProfileName;
        Ar << Data.CollisionEnabled;
        Ar << Data.ObjectType;
        Ar << Data.Responses;
        return Ar;
    }

    void SetProfileName(const FName& InName);
    FName GetProfileName() const { return CollisionProfileName; }
    void SetCollisionEnabled(const ECollisionEnabled::Type InCollisionEnabled);
    ECollisionEnabled::Type GetCollisionEnabled() const { return CollisionEnabled; }
    void SetObjectType(const ECollisionChannel InObjectType);
    ECollisionChannel GetObjectType() const { return ObjectType; }
    void SetAllResponses(const ECollisionResponse InResponse);
    void SetResponse(const ECollisionChannel InChannel, const ECollisionResponse InResponse);
    ECollisionResponse GetResponse(const ECollisionChannel InChannel) const;

    void SetResponsesMap(const TMap<TEnumAsByte<ECollisionChannel>, TEnumAsByte<ECollisionResponse>>& InResponses) { Responses = InResponses; }
    TMap<TEnumAsByte<ECollisionChannel>, TEnumAsByte<ECollisionResponse>> GetResponsesMap() const { return Responses; }

    void FillResponsesMap(const ECollisionResponse InResponses = ECR_Block);
    void FromPrimitiveComponent(const UPrimitiveComponent* InComponent);
    void ToPrimitiveComponent(UPrimitiveComponent* InComponent) const;
    
private:

    /* Type of collision enabled.
    * No Collision      : Will not create any representation in the physics engine. Cannot be used for spatial queries (raycasts, sweeps, overlaps) or simulation (rigid body, constraints). Best performance possible (especially for moving objects)
    * Query Only        : Only used for spatial queries (raycasts, sweeps, and overlaps). Cannot be used for simulation (rigid body, constraints). Useful for character movement and things that do not need physical simulation. Performance gains by keeping data out of simulation tree.
    * Physics Only      : Only used for physics simulation (rigid body, constraints). Cannot be used for spatial queries (raycasts, sweeps, overlaps). Useful for jiggly bits on characters that do not need per bone detection. Performance gains by keeping data out of query tree
    * Collision Enabled : Can be used for both spatial queries (raycasts, sweeps, overlaps) and simulation (rigid body, constraints).
    */
    UPROPERTY(EditAnywhere, Category = PrimitiveCollision)
        TEnumAsByte<ECollisionEnabled::Type> CollisionEnabled;

    /* Enum indicating what type of object this should be considered as when it moves */
    UPROPERTY(EditAnywhere, Category = PrimitiveCollision)
        TEnumAsByte<ECollisionChannel> ObjectType;

    UPROPERTY() TMap<TEnumAsByte<ECollisionChannel>, TEnumAsByte<ECollisionResponse>> Responses;
    UPROPERTY() FName CollisionProfileName;
};