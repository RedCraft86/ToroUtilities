// Copyright (C) RedCraft86. All Rights Reserved.

#include "Gameplay/Electronics/ElectricalSwitch.h"

AElectricalSwitch::AElectricalSwitch()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.TickGroup = TG_DuringPhysics;
}
