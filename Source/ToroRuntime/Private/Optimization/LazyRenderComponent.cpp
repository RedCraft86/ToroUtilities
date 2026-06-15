// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#include "Optimization/LazyRenderComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"

ULazyRenderComponent::ULazyRenderComponent(): bNullChecks(true)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bTickEvenWhenPaused = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.TickGroup = TG_DuringPhysics;
	PrimaryComponentTick.TickInterval = 1.0f;
}

void ULazyRenderComponent::AddRequest(const UObject* InRequester)
{
	Requests.AddRequest(InRequester);
}

void ULazyRenderComponent::RemoveRequest(const UObject* InRequester)
{
	Requests.RemoveRequest(InRequester);
}

void ULazyRenderComponent::OnRequestChanged(const bool bState) const
{
	if (AActor* Owner = GetOwner())
	{
		Owner->SetActorHiddenInGame(!bState);
	}
}

void ULazyRenderComponent::BeginPlay()
{
	Super::BeginPlay();
	Requests.OnRequestChanged.AddUObject(this, &ULazyRenderComponent::OnRequestChanged);
	SetComponentTickEnabled(bNullChecks);
}

void ULazyRenderComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* TickFunc)
{
	Super::TickComponent(DeltaTime, TickType, TickFunc);
	Requests.CleanupNulls();
}
