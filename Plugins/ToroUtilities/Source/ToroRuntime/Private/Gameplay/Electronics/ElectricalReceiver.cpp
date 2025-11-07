// Copyright (C) RedCraft86. All Rights Reserved.

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