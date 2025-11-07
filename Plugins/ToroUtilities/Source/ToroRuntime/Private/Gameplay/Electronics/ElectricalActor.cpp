// Copyright (C) RedCraft86. All Rights Reserved.

#include "Gameplay/Electronics/ElectricalActor.h"

AElectricalActor::AElectricalActor(): bRefreshOnInit(true), MinProviders(1), bPowered(false)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.TickGroup = TG_DuringPhysics;
}

void AElectricalActor::AddProvider(AElectricalActor* InProvider)
{
	if (MinProviders == 0) return;
	if (InProvider && !Providers.Contains(InProvider))
	{
		Providers.Add(InProvider);
		UpdatePoweredState(false);
	}
}

void AElectricalActor::RemoveProvider(AElectricalActor* InProvider)
{
	if (MinProviders == 0) return;
	if (InProvider && Providers.Remove(InProvider) > 0)
	{
		UpdatePoweredState(false);
	}
}

void AElectricalActor::BroadcastPoweredState()
{
	PoweredStateChanged(bPowered);
	PoweredStateChangedBP(bPowered);
	OnPoweredStateChanged.Broadcast(bPowered);
	OnPoweredStateChangedBP.Broadcast(bPowered);
}

void AElectricalActor::UpdatePoweredState(const bool bForceUpdate)
{
	const bool bTarget = CalcPoweredState();
	if (bForceUpdate || bPowered != bTarget)
	{
		bPowered = bTarget;
		BroadcastPoweredState();
	}
}

bool AElectricalActor::CalcPoweredState()
{
	if (MinProviders == 0)
	{
		return IsEnabled();
	}

	for (auto It = Providers.CreateIterator(); It; ++It)
	{
		if (!It->IsValid()) It.RemoveCurrent();
	}

	return IsEnabled() && Providers.Num() >= MinProviders;
}

void AElectricalActor::PoweredStateChanged(const bool bState)
{
	for (auto It = Targets.CreateIterator(); It; ++It)
	{
		const TWeakObjectPtr<AElectricalActor>& Target = *It;
		if (Target.IsValid())
		{
			bState ? Target->AddProvider(this) : Target->RemoveProvider(this);
		}
		else It.RemoveCurrent();
	}
}

void AElectricalActor::EnableStateChanged(const bool bState)
{
	UpdatePoweredState(true);
}

void AElectricalActor::BeginPlay()
{
	Super::BeginPlay();
	// Check bEnabled too since we don't want to double-fire if ToroActor already sent an update
	if (bEnabled && bRefreshOnInit)
	{
		UpdatePoweredState(true);
	}
}
