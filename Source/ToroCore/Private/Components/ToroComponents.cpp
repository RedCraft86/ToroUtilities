// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#include "Components/ToroComponents.h"
#include "Engine/World.h"
#include "TimerManager.h"

UToroActorComponent::UToroActorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	bAllowAnyoneToDestroyMe = true;
}

void UToroActorComponent::BeginPlay()
{
	OnConstruction();
	Super::BeginPlay();
#if WITH_EDITOR
	if (bIsEditorOnly)
	{
		SetComponentTickEnabled(false);
		GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
		{
			DestroyComponent();
		});
		return;
	}
#endif
}

#if WITH_EDITOR
void UToroActorComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	OnConstruction();
}
#endif

UToroSceneComponent::UToroSceneComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	bAllowAnyoneToDestroyMe = true;
}

void UToroSceneComponent::BeginPlay()
{
	OnConstruction();
	Super::BeginPlay();
#if WITH_EDITOR
	if (bIsEditorOnly)
	{
		SetComponentTickEnabled(false);
		GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
		{
			DestroyComponent();
		});
		return;
	}
#endif
}

#if WITH_EDITOR
void UToroSceneComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	OnConstruction();
}
#endif
