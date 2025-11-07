// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "Gameplay/Electronics/ElectricalReceiver.h"

AElectricalReceiver::AElectricalReceiver()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.TickGroup = TG_DuringPhysics;

	bRefreshOnInit = false;
}

void AElectricalReceiver::PoweredStateChanged(const bool bState)
{
	// Do nothing since there should not be targets
	// Super::PoweredStateChanged(bState);
}

#if WITH_EDITOR
void AElectricalReceiver::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	Targets.Empty();
}
#endif