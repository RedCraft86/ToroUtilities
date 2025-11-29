// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "Gameplay/Interaction/InteractionData.h"
#include "Gameplay/Interaction/InteractionInterface.h"

FInteractionInfo FInteractionInfo::Empty = FInteractionInfo(false, INVTEXT("INVALID"));

void FInteractionCache::Reset()
{
	bTriggered = false;
	SetTarget(nullptr);
	SetInfoTarget(nullptr);
}

void FInteractionCache::SetInfoTarget(AActor* InTarget)
{
	if (InTarget != InfoTarget.Get())
	{
		IInteractionInterface::SetMarkerState(InfoTarget.Get(), false);
		IInteractionInterface::SetMarkerState(InTarget, true);
		if (InTarget)
		{
			InfoTarget = InTarget;
		}
		else
		{
			InfoTarget.Reset();
		}
	}
}

void FInteractionCache::SetTarget(AActor* InTarget)
{
	if (InTarget != Target.Get())
	{
		if (InTarget)
		{
			Target = InTarget;
		}
		else
		{
			Target.Reset();
		}
	}
}

void FInteractionCache::StopInteract(AToroPlayerCharacter* Player)
{
	if (Target.IsValid() && bTriggered)
	{
		bTriggered = false;
		IInteractionInterface::EndInteract(Target.Get(), Player);
	}
}

void FInteractionCache::StartInteract(AToroPlayerCharacter* Player, const FHitResult& Hit)
{
	if (Target.IsValid() && !bTriggered)
	{
		bTriggered = true;
		IInteractionInterface::BeginInteract(Target.Get(), Player, Hit);
	}
}

FInteractionInfo FInteractionCache::GetInteractInfo(const FHitResult& Hit) const
{
	FInteractionInfo Result;
	IInteractionInterface::GetInteractInfo(Target.Get(), Hit, Result);
	return Result;
}
