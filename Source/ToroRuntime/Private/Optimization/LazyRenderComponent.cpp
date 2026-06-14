// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

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
