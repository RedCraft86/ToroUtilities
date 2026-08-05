// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#include "Components/LazyRenderComponent.h"

ULazyRenderComponent::ULazyRenderComponent()
	: Requests(TAccumulatorSet<TWeakObjectPtr<const UObject>>::Create(
		[](const TWeakObjectPtr<const UObject>& Object)
		{
			return Object.IsValid();
		}
	))
{
	PrimaryComponentTick.bCanEverTick = false;
	Requests->OnChanged.AddUObject(this, &ULazyRenderComponent::OnRequestChanged);
}

void ULazyRenderComponent::AddRenderRequest(const AActor* Target, const UObject* InRequester)
{
	if (Target && InRequester)
	{
		if (const ULazyRenderComponent* Component = Target->FindComponentByClass<ULazyRenderComponent>())
		{
			Component->AddRequest(InRequester);
		}
	}
}

void ULazyRenderComponent::RemoveRenderRequest(const AActor* Target, const UObject* InRequester)
{
	if (Target && InRequester)
	{
		if (const ULazyRenderComponent* Component = Target->FindComponentByClass<ULazyRenderComponent>())
		{
			Component->RemoveRequest(InRequester);
		}
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
		Owner->SetActorHiddenInGame(!Requests->IsEmpty());
	}
}
