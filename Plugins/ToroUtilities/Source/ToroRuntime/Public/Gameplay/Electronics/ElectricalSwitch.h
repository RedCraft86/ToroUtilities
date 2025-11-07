// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "ElectricalActor.h"
#include "ElectricalSwitch.generated.h"

UCLASS(Abstract)
class TORORUNTIME_API AElectricalSwitch final : public AElectricalActor
{
	GENERATED_BODY()

public:

	AElectricalSwitch();
};
