// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "Components/RefCullComponent.h"

URefCullComponent::URefCullComponent(): bNullChecks(false), bAffectTicking(false)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.TickGroup = TG_DuringPhysics;
	PrimaryComponentTick.TickInterval = 0.5f;
}

void URefCullComponent::AddRequest(const UObject* InRequester)
{
	if (InRequester && !Requests.Contains(InRequester))
	{
		Requests.Add(InRequester);
		UpdateCullingState();
	}
}

void URefCullComponent::RemoveRequest(const UObject* InRequester)
{
	if (InRequester && Requests.Remove(InRequester) > 0)
	{
		UpdateCullingState();
	}
}

void URefCullComponent::UpdateCullingState()
{
	for (auto It = Requests.CreateIterator(); It; ++It)
	{
		if (!It->IsValid()) It.RemoveCurrent();
	}

	AActor* Owner = GetOwner();
	Owner->SetActorHiddenInGame(Requests.IsEmpty());
	if (bAffectTicking && Requests.IsEmpty())
	{
		OriginalTickState = Owner->IsActorTickEnabled();
		Owner->SetActorTickEnabled(false);
	}
	else if (OriginalTickState.IsSet())
	{
		Owner->SetActorTickEnabled(OriginalTickState.GetValue());
		OriginalTickState.Reset();
		
	}
}

void URefCullComponent::BeginPlay()
{
	Super::BeginPlay();
	SetComponentTickEnabled(bNullChecks);
}

void URefCullComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* TickFunc)
{
	Super::TickComponent(DeltaTime, TickType, TickFunc);
	UpdateCullingState();
}
