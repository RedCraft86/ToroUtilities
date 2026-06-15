// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "GameFramework/Actor.h"
#include "DataTypes/WireShapes.h"
#include "Components/ToroComponents.h"
#include "EdShapeComponent.generated.h"

/**
 * A specialized utility component for drawing structured wireframe shapes in the editor.
 * It manages collections of points, lines, and complex volumes (spheres, capsules, nav paths) 
 * through a TMap-based system, allowing for named debug geometry.
 * 
 * Note: All visualization data is stripped in non-editor builds.
 */
UCLASS(MinimalAPI, NotBlueprintable, DisplayName = "Editor Shapes", ClassGroup = (Editor), meta = (BlueprintSpawnableComponent))
class UEdShapeComponent final : public UToroActorComponent
{
	GENERATED_BODY()

public:

	UEdShapeComponent();

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shapes)
		TMap<FName, FWirePointData> WirePoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shapes)
		TMap<FName, FWireLineData> WireLines;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shapes)
		TMap<FName, FWireCircleData> WireCircles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shapes)
		TMap<FName, FWireArcData> WireArcs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shapes)
		TMap<FName, FWireBoxData> WireBoxes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shapes)
		TMap<FName, FWireSphereData> WireSpheres;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shapes)
		TMap<FName, FWireCylinderData> WireCylinders;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shapes)
		TMap<FName, FWireCapsuleData> WireCapsules;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shapes)
		TMap<FName, FWireNavPathData> WireNavPaths;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shapes)
		TMap<FName, FWireStringData> WireStrings;
#endif

#if WITH_EDITOR
	TOROCORE_API FVector TransformLocation(const FVector& LocalLocation) const
	{
		return GetOwner()->GetActorTransform().TransformPositionNoScale(LocalLocation);
	}
	TOROCORE_API FRotator TransformRotation(const FRotator& LocalRotation) const
	{
		return GetOwner()->GetActorTransform().TransformRotation(LocalRotation.Quaternion()).Rotator();
	}
	TOROCORE_API FVector AsForwardVector(const FRotator& LocalRotation) const
	{
		return TransformRotation(LocalRotation).Vector();
	}
	TOROCORE_API FVector AsRightVector(const FRotator& LocalRotation) const
	{
		return FRotationMatrix(TransformRotation(LocalRotation)).GetScaledAxis(EAxis::Y);
	}
	TOROCORE_API FVector AsUpVector(const FRotator& LocalRotation) const
	{
		return FRotationMatrix(TransformRotation(LocalRotation)).GetScaledAxis(EAxis::Z);
	}

	TOROCORE_API void UpdateNavPoints();

protected:

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* TickFunc) override;
#endif
};
