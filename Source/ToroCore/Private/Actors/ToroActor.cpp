// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#include "Actors/ToroActor.h"

AToroActor::AToroActor(): bEnabled(true), bStartWithCollision(true)
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

#if WITH_EDITOR
	DEFINE_ICON_COMPONENT
#endif

	SetCanBeDamaged(false);

	UniqueId.RegenerateGuid();
}

void AToroActor::SetEnabledState_Implementation(const bool bInEnabled)
{
	if (bEnabled != bInEnabled)
	{
		bEnabled = bInEnabled;
		BroadcastStateChanged();
	}
}

void AToroActor::BroadcastStateChanged()
{
	EnableStateChanged(bEnabled);
	EnableStateChangedBP(bEnabled);
	OnEnableStateChanged.Broadcast(bEnabled);
	OnEnableStateChangedBP.Broadcast(bEnabled);
}

void AToroActor::EnableStateChanged(const bool bState)
{
	SetActorHiddenInGame(!bState);
	SetActorEnableCollision(bState);
	SetActorTickEnabled(PrimaryActorTick.bStartWithTickEnabled && bState);
}

void AToroActor::BeginPlay()
{
	Super::BeginPlay();
	SetActorEnableCollision(bStartWithCollision);
	if (!bEnabled) BroadcastStateChanged(); // Notify if disabled to change defaults which are assumed enabled
}

#if WITH_EDITOR
void AToroActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	UniqueId.RegenerateGuid(true);
}
#endif
