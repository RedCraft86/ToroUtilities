// Copyright (C) RedCraft86. All Rights Reserved.

#include "Gameplay/Interaction/InteractableActor.h"

AInteractableActor::AInteractableActor()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	Marker = CreateDefaultSubobject<UInteractionMarker>("Marker");
	Marker->SetupAttachment(GetRootComponent());

#if WITH_EDITOR
	UPDATE_VISUAL_MAX_COMP(2);
#endif
}

void AInteractableActor::SetMarkerState_Implementation(const bool bVisible)
{
	Marker->SetVisibility(bVisible);
}

bool AInteractableActor::GetInteractInfo_Implementation(const FHitResult& Hit, FInteractionInfo& Info)
{
	Info = Interaction;
	return IsEnabled() && Interaction.bEnabled;
}

void AInteractableActor::OnBeginInteract_Implementation(AToroPlayerCharacter* Player, const FHitResult& Hit)
{
	OnInteracted.Broadcast(this, Player);
	OnInteractedBP.Broadcast(this, Player);
}
