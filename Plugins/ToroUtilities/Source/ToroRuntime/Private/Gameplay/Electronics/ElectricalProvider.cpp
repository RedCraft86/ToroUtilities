// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "Gameplay/Electronics/ElectricalProvider.h"

AElectricalProvider::AElectricalProvider()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.TickGroup = TG_DuringPhysics;

	MinProviders = 0;
}

bool AElectricalProvider::CalcPoweredState()
{
	return IsEnabled();
}

#if WITH_EDITOR
void AElectricalProvider::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	Providers.Empty();
	MinProviders = 0;
}
#endif
