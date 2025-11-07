// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "DataTypes/PrimitiveData.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ToroPrimitiveUtils.generated.h"

UCLASS(DisplayName = "Primitive Utils")
class TOROCORE_API UToroPrimitiveUtils final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

    UFUNCTION(BlueprintCallable, Category = "StaticMesh|MeshData")
        static void FillMaterials(UPARAM(ref) FStaticMeshData& Target);

    UFUNCTION(BlueprintPure, Category = "StaticMesh|MeshData", DisplayName = "Is Valid")
        static bool IsValid_MeshData(const FStaticMeshData& Target);

    UFUNCTION(BlueprintPure, Category = "Components|StaticMesh", meta = (DefaultToSelf = "Target"))
        static FStaticMeshData GetStaticMeshData(const UStaticMeshComponent* Target);

    UFUNCTION(BlueprintCallable, Category = "Components|StaticMesh", meta = (DefaultToSelf = "Target"))
        static void SetStaticMeshData(UStaticMeshComponent* Target, const FStaticMeshData& Data);

    UFUNCTION(BlueprintPure, Category = "Collision|PrimitiveCollision", meta = (BlueprintThreadSafe, AutoCreateRefTerm = "InResponses", AdvancedDisplay = "InCollisionEnabled, InObjectType, InResponses"))
        static FPrimitiveCollision Make_PrimitiveCollision(const FName InProfileName, const TEnumAsByte<ECollisionEnabled::Type> InCollisionEnabled,
            const TEnumAsByte<ECollisionChannel> InObjectType, const TMap<TEnumAsByte<ECollisionChannel>, TEnumAsByte<ECollisionResponse>>& InResponses);

    UFUNCTION(BlueprintPure, Category = "Collision|PrimitiveCollision", meta = (BlueprintThreadSafe, AdvancedDisplay = "ProfileName, CollisionEnabled, ObjectType, Responses"))
        static void Break_PrimitiveCollision(const FPrimitiveCollision& Target, FName& ProfileName, TEnumAsByte<ECollisionEnabled::Type>& CollisionEnabled,
            TEnumAsByte<ECollisionChannel>& ObjectType, TMap<TEnumAsByte<ECollisionChannel>, TEnumAsByte<ECollisionResponse>>& Responses);

    UFUNCTION(BlueprintCallable, Category = "Collision|PrimitiveCollision")
        static void SetCollisionProfileName(UPARAM(ref) FPrimitiveCollision& Target, const FName InProfileName);

    UFUNCTION(BlueprintPure, Category = "Collision|PrimitiveCollision", meta = (BlueprintAutocast = true))
        static FName GetCollisionProfileName(const FPrimitiveCollision& Target);

    UFUNCTION(BlueprintCallable, Category = "Collision|PrimitiveCollision")
        static void SetAllCollisionResponses(UPARAM(ref) FPrimitiveCollision& Target, const ECollisionResponse InResponse);
    
    UFUNCTION(BlueprintCallable, Category = "Collision|PrimitiveCollision")
        static void SetCollisionResponse(UPARAM(ref) FPrimitiveCollision& Target, const ECollisionChannel InChannel, const ECollisionResponse InResponse);

    UFUNCTION(BlueprintPure, Category = "Collision|PrimitiveCollision")
        static ECollisionResponse GetCollisionResponse(const FPrimitiveCollision& Target, const ECollisionChannel InChannel);

    UFUNCTION(BlueprintCallable, Category = "Collision|PrimitiveCollision")
        static void SetCollisionEnabled(UPARAM(ref) FPrimitiveCollision& Target, const ECollisionEnabled::Type InEnabled);

    UFUNCTION(BlueprintPure, Category = "Collision|PrimitiveCollision")
        static ECollisionEnabled::Type GetCollisionEnabled(const FPrimitiveCollision& Target);

    UFUNCTION(BlueprintCallable, Category = "Collision|PrimitiveCollision")
        static void SetCollisionObjectType(UPARAM(ref) FPrimitiveCollision& Target, const ECollisionChannel InObjectType);

    UFUNCTION(BlueprintPure, Category = "Collision|PrimitiveCollision")
        static ECollisionChannel GetCollisionObjectType(const FPrimitiveCollision& Target);

    UFUNCTION(BlueprintPure, Category = Collision, meta = (DefaultToSelf = "Target"))
        static FPrimitiveCollision GetPrimitiveCollision(const UPrimitiveComponent* Target);

    UFUNCTION(BlueprintCallable, Category = Collision, meta = (DefaultToSelf = "Target"))
        static void SetPrimitiveCollision(UPrimitiveComponent* Target, const FPrimitiveCollision& Data);
};
