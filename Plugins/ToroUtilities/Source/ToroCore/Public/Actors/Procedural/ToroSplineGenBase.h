// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroProcGenBase.h"
#include "Components/SplineComponent.h"
#include "ToroSplineGenBase.generated.h"

UCLASS(Abstract)
class TOROCORE_API AToroSplineGenBase : public AToroProcGenBase
{
	GENERATED_BODY()

public:

	AToroSplineGenBase();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<USplineComponent> SplineComponent;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (DisplayPriority = -10))
		bool bClosedLoop;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (DisplayPriority = -10))
		TEnumAsByte<ESplinePointType::Type> SplineType;

#if WITH_EDITOR
protected:
	virtual void OnConstruction(const FTransform& Transform) override;
#endif
};
