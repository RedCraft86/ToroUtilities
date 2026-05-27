// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "Components/Optimization/LazyRenderComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"

ULazyRenderComponent::ULazyRenderComponent(): bNullChecks(false)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.bTickEvenWhenPaused = true;
	PrimaryComponentTick.TickGroup = TG_DuringPhysics;
	PrimaryComponentTick.TickInterval = 1.0f;
}

void ULazyRenderComponent::AddRequest(const UObject* InRequester)
{
	if (InRequester && !Requests.Contains(InRequester))
	{
		Requests.Add(InRequester);
		UpdateRenderState();
	}
}

void ULazyRenderComponent::RemoveRequest(const UObject* InRequester)
{
	if (InRequester && Requests.Remove(InRequester) > 0)
	{
		UpdateRenderState();
	}
}

void ULazyRenderComponent::UpdateRenderState()
{
	for (auto It = Requests.CreateIterator(); It; ++It)
	{
		if (!It->IsValid()) It.RemoveCurrent();
	}

	if (AActor* Owner = GetOwner())
	{
		Owner->SetActorHiddenInGame(Requests.IsEmpty());
	}
}

void ULazyRenderComponent::BeginPlay()
{
	Super::BeginPlay();
	SetComponentTickEnabled(bNullChecks);
}

void ULazyRenderComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* TickFunc)
{
	Super::TickComponent(DeltaTime, TickType, TickFunc);
	if (bNullChecks)
	{
		UpdateRenderState();
	}
}
