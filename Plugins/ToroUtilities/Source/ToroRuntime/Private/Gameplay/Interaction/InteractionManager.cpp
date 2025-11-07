// Copyright (C) RedCraft86. All Rights Reserved.

#include "Gameplay/Interaction/InteractionManager.h"
#include "Gameplay/Interaction/InteractionInterface.h"
#include "UserInterface/ToroWidgetManager.h"

UInteractionManager::UInteractionManager(): bEnabled(false), bInteracting(false)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = TG_DuringPhysics;
	PrimaryComponentTick.TickInterval = 0.05f;
}

void UInteractionManager::SetEnabled(const bool bInEnabled)
{
	if (bEnabled != bInEnabled)
	{
		bEnabled = bInEnabled;
		if (!bEnabled && bInteracting)
		{
			SetInteracting(false);
		}

		if (UInteractionWidget* Widget = GetInteractionWidget())
		{
			bEnabled ? Widget->PushWidget() : Widget->PopWidget();
		}
	}
}

void UInteractionManager::SetInteracting(const bool bInInteracting)
{
	const bool bTarget = bInInteracting && CanInteract();
	if (bInteracting != bTarget)
	{
		bInteracting = bTarget;
		if (!bInteracting)
		{
			CleanupCache();
		}
	}
}

UInteractionWidget* UInteractionManager::GetInteractionWidget()
{
	if (!InteractionWidget)
	{
		InteractionWidget = AToroWidgetManager::GetWidget<UInteractionWidget>(this);
	}
	return InteractionWidget;
}

void UInteractionManager::CleanupCache()
{
	if (InteractCache.GetTarget())
	{
		InteractCache.StopInteract(PlayerChar);
		InteractCache.Reset();

		if (const UInteractionWidget* Widget = GetInteractionWidget())
		{
			Widget->SetInteractionInfo(FInteractionInfo::Empty);
		}
	}
}

void UInteractionManager::BeginPlay()
{
	Super::BeginPlay();
	PlayerChar = GetOwner<AToroPlayerCharacter>();
}

void UInteractionManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* TickFunc)
{
	Super::TickComponent(DeltaTime, TickType, TickFunc);

	if (!PlayerChar) return;
	if (!CanInteract())
	{
		CleanupCache();
		return;
	}

	const FHitResult& HitResult = HandleTrace.Execute();
	AActor* HitActor = HitResult.GetActor();

	FInteractionInfo Info;
	if (!IInteractionInterface::GetInteractInfo(HitActor, HitResult, Info))
	{
		CleanupCache();
		return;
	}

	if (const UInteractionWidget* Widget = GetInteractionWidget())
	{
		Widget->SetInteractionInfo(Info);
	}

	if (!bInteracting) return;
	if (InteractCache.GetTarget() != HitActor)
	{
		InteractCache.StopInteract(PlayerChar);

		InteractCache.SetTarget(HitActor);
		InteractCache.StartInteract(PlayerChar, HitResult);
	}
}
