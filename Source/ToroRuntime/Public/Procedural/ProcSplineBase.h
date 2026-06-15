// Copyright (C) 2026 RedCraft86. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "ProceduralBase.h"
#include "Components/SplineComponent.h"
#include "ProcSplineBase.generated.h"

/**
 * Base class for actors that generate content along a spline path.
 */
UCLASS(Abstract)
class TORORUNTIME_API AProcSplineBase : public AProceduralBase
{
	GENERATED_BODY()

public:

	AProcSplineBase();

protected:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<USplineComponent> SplineComponent;

	/** Whether the spline should connect its start and end points to form a loop. */
	UPROPERTY(EditAnywhere, Category = Settings, meta = (DisplayPriority = -10))
		bool bClosedLoop;

	/** Toggle to force all spline points to use a specific interpolation type. */
	UPROPERTY(EditAnywhere, Category = Settings, meta = (InlineEditConditionToggle, DisplayPriority = -10))
		bool bRestrictSplineType;

	/** The point type (Linear, Curve, etc.) to enforce on the spline if bRestrictSplineType is true. */
	UPROPERTY(EditAnywhere, Category = Settings, meta = (EditCondition = "bRestrictSplineType", DisplayPriority = -10))
		TEnumAsByte<ESplinePointType::Type> SplineType;

#if WITH_EDITOR
	virtual void OnConstruction(const FTransform& Transform) override;
#endif
};