// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#pragma once

#include "DataTypes/WireShapes.h"
#include "Components/ActorComponent.h"
#include "WireShapeComponent.generated.h"

/**
 * Component to render ComponentVisualizer shapes when selected in the editor.
 * @note This component is editor-only and will be removed at game runtime.
 */
UCLASS(MinimalAPI, NotBlueprintable, NotBlueprintType, ClassGroup = (Editor), meta = (BlueprintSpawnableComponent))
class UWireShapeComponent final : public UActorComponent
{
	GENERATED_BODY()

public:

	UWireShapeComponent();

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
private:

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* TickFunc) override;
#endif
};
