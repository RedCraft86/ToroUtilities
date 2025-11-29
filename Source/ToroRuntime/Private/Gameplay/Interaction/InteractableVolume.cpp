// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "Gameplay/Interaction/InteractableVolume.h"

AInteractableVolume::AInteractableVolume()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	Marker = CreateDefaultSubobject<UInteractionMarker>("Marker");
	Marker->SetupAttachment(GetRootComponent());

	GetBrushComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
}

void AInteractableVolume::SetMarkerState_Implementation(const bool bHideMarker)
{
	Marker->SetHiddenInGame(bHideMarker);
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
