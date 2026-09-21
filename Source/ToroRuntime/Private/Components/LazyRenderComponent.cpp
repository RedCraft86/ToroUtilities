// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#include "Components/LazyRenderComponent.h"

namespace
{
	ULazyRenderComponent* FindComponentOnActor(const AActor* Actor)
	{
		if (IsValid(Actor))
		{
			TArray<ULazyRenderComponent*> Components;
			Actor->GetComponents<ULazyRenderComponent>(Components);
			for (ULazyRenderComponent* Component : Components)
			{
				if (Component && Component->IsActive())
				{
					return Component;
				}
			}
		}

		return nullptr;
	}
}

ULazyRenderComponent::ULazyRenderComponent()
	: Requests(TAccumulatorSet<TWeakObjectPtr<const UObject>>::Create(
		[](const TWeakObjectPtr<const UObject>& Object)
		{
			return Object.IsValid();
		}
	))
{
	PrimaryComponentTick.bCanEverTick = false;
	MaxInstancesPerActor = 1;
	bAutoActivate = true;
}

void ULazyRenderComponent::AddRenderRequest(const AActor* Target, const UObject* InRequester)
{
	const ULazyRenderComponent* Component = FindComponentOnActor(Target);
	if (Component && IsValid(InRequester))
	{
		Component->AddRequest(InRequester);
	}
}

void ULazyRenderComponent::RemoveRenderRequest(const AActor* Target, const UObject* InRequester)
{
	const ULazyRenderComponent* Component = FindComponentOnActor(Target);
	if (Component && IsValid(InRequester))
	{
		Component->RemoveRequest(InRequester);
	}
}

void ULazyRenderComponent::AddRequest(const UObject* InRequester) const
{
	Requests->Add(InRequester);
}

void ULazyRenderComponent::RemoveRequest(const UObject* InRequester) const
{
	Requests->Remove(InRequester);
}

void ULazyRenderComponent::OnRequestChanged() const
{
	if (AActor* Owner = GetOwner())
	{
		Owner->SetActorHiddenInGame(IsActive() && Requests->IsEmpty());
	}
}

void ULazyRenderComponent::Activate(bool bReset)
{
	Super::Activate(bReset);
	Requests->OnChanged.RemoveAll(this);
	Requests->OnChanged.AddUObject(this, &ULazyRenderComponent::OnRequestChanged);
	OnRequestChanged();
}

void ULazyRenderComponent::Deactivate()
{
	Super::Deactivate();
	Requests->OnChanged.RemoveAll(this);
	OnRequestChanged();
}
