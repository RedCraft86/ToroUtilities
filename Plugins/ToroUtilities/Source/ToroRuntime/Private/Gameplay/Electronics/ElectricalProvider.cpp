// Copyright (C) RedCraft86. All Rights Reserved.

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
