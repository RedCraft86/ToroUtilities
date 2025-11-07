// Copyright (C) RedCraft86. All Rights Reserved.

#include "Gameplay/Interaction/InteractionData.h"
#include "Gameplay/Interaction/InteractionInterface.h"

FInteractionInfo FInteractionInfo::Empty = FInteractionInfo(false, INVTEXT("INVALID"));

void FInteractionCache::Reset()
{
	bTriggered = false;
	SetTarget(nullptr);
}

void FInteractionCache::SetTarget(AActor* InTarget)
{
	IInteractionInterface::SetMarkerState(Target.Get(), true);
	if (InTarget)
	{
		Target = InTarget;
		IInteractionInterface::SetMarkerState(InTarget, false);
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
