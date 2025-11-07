// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "WireShapes.generated.h"

USTRUCT(BlueprintType)
struct TOROCORE_API FWireShapeBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireShape, meta = (HideAlphaChannel = true))
        FLinearColor Color;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireShape)
        bool bRenderOnTopOfEverything;

    FWireShapeBase(): Color(FColor::Cyan), bRenderOnTopOfEverything(false) {}
};

USTRUCT(BlueprintType)
struct TOROCORE_API FWirePointData final : public FWireShapeBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireShape)
        FVector Location;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireShape, meta = (ClampMin = 1.0f, UIMin = 1.0f))
        float Size;

    FWirePointData(): Location(FVector(0.0f)), Size(2.0f) {}
};

USTRUCT(BlueprintType)
struct TOROCORE_API FWireLineData final : public FWireShapeBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireShape)
        FVector Start;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireShape)
        FVector End;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireShape, meta = (ClampMin = 1.0f, UIMin = 1.0f))
        float Thickness;

    FWireLineData(): Start(FVector(0.0f)), End(FVector(0.0f)), Thickness(1.0f) {}
};

USTRUCT(BlueprintType)
struct TOROCORE_API FWireCircleData final : public FWireShapeBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireShape)
        FVector Location;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireShape)
        FRotator Rotation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireShape, meta = (ClampMin = 1.0f, UIMin = 1.0f))
        float Radius;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireShape, meta = (ClampMin = 1, UIMin = 1))
        uint8 NumOfSides;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireShape, meta = (ClampMin = 1.0f, UIMin = 1.0f))
        float Thickness;

    FWireCircleData()
        : Location(FVector(0.0f))
        , Rotation(FRotator(0.0f))
        , Radius(32.0f)
        , NumOfSides(16)
        , Thickness(1.0f)
    {}
};

USTRUCT(BlueprintType)
struct TOROCORE_API FWireArcData final : public FWireShapeBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireShape)
        FVector Location;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireShape)
        FRotator Rotation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireShape, meta = (ClampMin = -360.0f, UIMin = -360.0f, ClampMax = 360.0f, UIMax = 360.0f))
        float MinAngle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireShape, meta = (ClampMin = -360.0f, UIMin = -360.0f, ClampMax = 360.0f, UIMax = 360.0f))
        float MaxAngle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireShape, meta = (ClampMin = 1, UIMin = 1))
        float Radius;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireShape, meta = (ClampMin = 1.0f, UIMin = 1.0f))
        uint8 Sections;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireShape, meta = (ClampMin = 1.0f, UIMin = 1.0f))
        float Thickness;

    FWireArcData()
        : Location(FVector(0.0f))
        , Rotation(FRotator(0.0f))
        , MinAngle(0.0f)
        , MaxAngle(90.0f)
        , Radius(32.0f)
        , Sections(8)
        , Thickness(1.0f)
    {}
};

USTRUCT(BlueprintType)
struct TOROCORE_API FWireBoxData final : public FWireShapeBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireShape)
        FVector Location;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireShape)
        FRotator Rotation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireShape, meta = (ClampMin = 1.0f, UIMin = 1.0f))
        FVector Extents;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireShape, meta = (ClampMin = 1.0f, UIMin = 1.0f))
        float Thickness;

    FWireBoxData()
        : Location(FVector(0.0f))
        , Rotation(FRotator(0.0f))
        , Extents(FVector(32.0f))
        , Thickness(1.0f)
    {}
};

USTRUCT(BlueprintType)
struct TOROCORE_API FWireSphereData final : public FWireShapeBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireShape)
        FVector Location;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireShape, meta = (ClampMin = 1.0f, UIMin = 1.0f))
        float Radius;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireShape, meta = (ClampMin = 1, UIMin = 1))
        uint8 NumOfSides;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireShape, meta = (ClampMin = 1.0f, UIMin = 1.0f))
        float Thickness;

    FWireSphereData()
        : Location(FVector(0.0f))
        , Radius(32.0f)
        , NumOfSides(16)
        , Thickness(1.0f)
    {}
};

USTRUCT(BlueprintType)
struct TOROCORE_API FWireCylinderData final : public FWireShapeBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireShape)
        FVector Location;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireShape)
        FRotator Rotation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireShape, meta = (ClampMin = 1.0f, UIMin = 1.0f))
        float Radius;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireShape, meta = (ClampMin = 1.0f, UIMin = 1.0f))
        float HalfHeight;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireShape, meta = (ClampMin = 1, UIMin = 1))
        uint8 NumOfSides;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireShape, meta = (ClampMin = 1.0f, UIMin = 1.0f))
        float Thickness;

    FWireCylinderData()
        : Location(FVector(0.0f))
        , Rotation(FRotator(0.0f))
        , Radius(32.0f)
        , HalfHeight(64.0f)
        , NumOfSides(12)
        , Thickness(1.0f)
    {}
};

USTRUCT(BlueprintType)
struct TOROCORE_API FWireCapsuleData final : public FWireShapeBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireShape)
        FVector Location;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireShape)
        FRotator Rotation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireShape, meta = (ClampMin = 1.0f, UIMin = 1.0f))
        float Radius;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireShape, meta = (ClampMin = 1.0f, UIMin = 1.0f))
        float HalfHeight;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireShape, meta = (ClampMin = 1, UIMin = 1))
        uint8 NumOfSides;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireShape, meta = (ClampMin = 1.0f, UIMin = 1.0f))
        float Thickness;

    FWireCapsuleData()
        : Location(FVector(0.0f))
        , Rotation(FRotator(0.0f))
        , Radius(32.0f)
        , HalfHeight(64.0f)
        , NumOfSides(16)
        , Thickness(1.0f)
    {}
};

USTRUCT(BlueprintType)
struct TOROCORE_API FWireNavPathData final : public FWireShapeBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WirePath)
        TArray<FVector> Targets;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WirePath, meta = (ClampMin = 1.0f, UIMin = 1.0f))
        float LineThickness;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WirePath, meta = (ClampMin = 1.0f, UIMin = 1.0f))
        float PointSize;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WirePath, meta = (HideAlphaChannel = true))
        FLinearColor PointColor;

    FWireNavPathData()
        : Targets({})
        , LineThickness(4.0f)
        , PointSize(16.0f)
        , PointColor(FColor::Green)
    {}
};

USTRUCT(BlueprintType)
struct TOROCORE_API FWireStringData final
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireString)
        FString String;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireString)
        FVector Location;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireString)
        FLinearColor Color;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireString, meta = (ClampMin = 1, UIMin = 1))
        uint8 FontScale;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WireString)
        bool bDrawShadow;

    FWireStringData()
        : String(TEXT("Wire_String"))
        , Location(FVector(0.0f))
        , Color(FColor::Green)
        , FontScale(12)
        , bDrawShadow(true)
    {}
};