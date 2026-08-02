// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#include "Components/LazyRenderComponent.h"

ULazyRenderComponent::ULazyRenderComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bTickEvenWhenPaused = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.TickGroup = TG_DuringPhysics;
	PrimaryComponentTick.TickInterval = 1.0f;
}

void ULazyRenderComponent::AddRenderRequest(const AActor* Target, const UObject* InRequester)
{
	if (Target && InRequester)
	{
		if (ULazyRenderComponent* Component = Target->GetComponentByClass<ULazyRenderComponent>())
		{
			Component->AddRequest(InRequester);
		}
	}
}

void ULazyRenderComponent::RemoveRenderRequest(const AActor* Target, const UObject* InRequester)
{
	if (Target && InRequester)
	{
		if (ULazyRenderComponent* Component = Target->GetComponentByClass<ULazyRenderComponent>())
		{
			Component->RemoveRequest(InRequester);
		}
	}
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
}

void ULazyRenderComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* TickFunc)
{
	Super::TickComponent(DeltaTime, TickType, TickFunc);
	Requests.CleanupNulls();
}
