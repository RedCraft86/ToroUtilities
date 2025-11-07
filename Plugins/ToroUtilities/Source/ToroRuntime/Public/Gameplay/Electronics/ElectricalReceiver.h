// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "ElectricalActor.h"
#include "ElectricalReceiver.generated.h"

UCLASS(Abstract)
class TORORUNTIME_API AElectricalReceiver final : public AElectricalActor
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
