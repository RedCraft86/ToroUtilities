// Copyright (C) RedCraft86. All Rights Reserved.

#include "Gameplay/Interaction/InteractableVolume.h"

AInteractableVolume::AInteractableVolume()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	Marker = CreateDefaultSubobject<UInteractionMarker>("Marker");
	Marker->SetupAttachment(GetRootComponent());
}

void AInteractableVolume::SetMarkerState_Implementation(const bool bVisible)
{
	Marker->SetVisibility(bVisible);
}

bool AInteractableVolume::GetInteractInfo_Implementation(const FHitResult& Hit, FInteractionInfo& Info)
{
	Info = Interaction;
	return IsEnabled() && Interaction.bEnabled;
}

void AInteractableVolume::OnBeginInteract_Implementation(AToroPlayerCharacter* Player, const FHitResult& Hit)
{
	OnInteracted.Broadcast(this, Player);
	OnInteractedBP.Broadcast(this, Player);
}
