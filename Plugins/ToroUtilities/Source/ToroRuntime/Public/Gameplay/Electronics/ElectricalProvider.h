// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ElectricalActor.h"
#include "ElectricalProvider.generated.h"

UCLASS(Abstract)
class TORORUNTIME_API AElectricalProvider : public AElectricalActor
{
	GENERATED_BODY()

public:

	AElectricalProvider();

protected:

	virtual bool CalcPoweredState() override;
#if WITH_EDITOR
	virtual void OnConstruction(const FTransform& Transform) override;
#endif
};
