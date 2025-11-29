// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "Components/CullingComponent.h"
#include "Interfaces/ActorStateInterface.h"
#include "Helpers/LoggingMacros.h"
#include "ToroCore.h"

UCullingComponent::UCullingComponent()
	: bAffectVisibility(true), bAffectTicking(false), bAffectState(false)
	, bCulled(false), bCachedTick(false)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.TickGroup = TG_DuringPhysics;
	PrimaryComponentTick.TickInterval = 1.0f;
#if WITH_EDITOR
	bTickInEditor = false;
#endif
}

UCullingComponent* UCullingComponent::Get(const AActor* Target)
{
	return Target ? Target->FindComponentByClass<UCullingComponent>() : nullptr;
}

void UCullingComponent::AddRenderRequest(const UObject* Object)
{
	if (Object && !Requests.Contains(Object))
	{
		Requests.Add(Object);
		ProcessRequests();
	}
}

void UCullingComponent::RemoveRenderRequest(const UObject* Object)
{
	if (Object && Requests.Remove(Object) > 0)
	{
		ProcessRequests();
	}
}

void UCullingComponent::ProcessRequests()
{
#if WITH_EDITOR
	if (!FApp::IsGame()) return;
#endif
	if (!IsComponentTickEnabled()) return;
	if (AActor* OwnerActor = GetOwner())
	{
		bCulled = Requests.IsEmpty();
		if (bAffectVisibility)
		{
			OwnerActor->SetActorHiddenInGame(bCulled);
		}

		if (bAffectTicking)
		{
			if (bCulled)
			{
				bCachedTick = OwnerActor->IsActorTickEnabled();
				OwnerActor->SetActorTickEnabled(false);
			}
			else
			{
				OwnerActor->SetActorTickEnabled(bCachedTick);
			}
		}

		if (bAffectState)
		{
			IActorStateInterface::SetEnabled(OwnerActor, !bCulled);
		}
	}
}

void UCullingComponent::BeginPlay()
{
	Super::BeginPlay();
	TArray<UCullingComponent*> Components;
	GetOwner()->GetComponents<UCullingComponent>(Components);
	if (Components.Num() > 1 && Components.Find(this) > 0)
	{
		UE_LOG_WARNING(LogToroCore, 1.0f,
			TEXT("Multiple CullingComponents found on %s. Removing duplicate %s."),
			*GetOwner()->GetName(), *GetName());

		SetComponentTickEnabled(false);
		DestroyComponent();
	}
	else
	{
		ProcessRequests();
	}
}

void UCullingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* TickFunc)
{
	Super::TickComponent(DeltaTime, TickType, TickFunc);

	bool bChanged = false;
	for (auto It = Requests.CreateIterator(); It; ++It)
	{
		if (!It->IsValid())
		{
			It.RemoveCurrent();
			bChanged = true;
		}
	}

	if (bChanged) ProcessRequests();
}
