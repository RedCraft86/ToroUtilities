// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "ElectricalActor.h"
#include "Components/CullingComponent.h"
#include "ElectricalReceiver.generated.h"

UCLASS(Abstract)
class TORORUNTIME_API AElectricalReceiver : public AElectricalActor
{
	GENERATED_BODY()

public:

	AElectricalReceiver();

protected:

	virtual void PoweredStateChanged(const bool bState) override;
#if WITH_EDITOR
	virtual void OnConstruction(const FTransform& Transform) override;
#endif
};

UCLASS(Abstract)
class TORORUNTIME_API AElectricalLight final : public AElectricalReceiver
{
	GENERATED_BODY()
	
public:

	AElectricalLight();

	UPROPERTY(VisibleDefaultsOnly, Category = Subobjects)
		TObjectPtr<UCullingComponent> Culling;
};