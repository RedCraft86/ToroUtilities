// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#include "MiscObjects/ToroObject.h"
#include "Helpers/WorldGetter.h"

UToroObject::UToroObject()
	: FTickableGameObject(ETickableTickType::Never), bCanTick(false), bTickWhenPaused(false)
{
}

void UToroObject::SetTickEnabled(const bool bEnabled)
{
	bCanTick = bEnabled;
}

bool UToroObject::IsTickEnabled() const
{
	return bCanTick;
}

UWorld* UToroObject::GetWorld() const
{
#if WITH_EDITOR
	return FApp::IsGame() ? FWorldGetter::Get(GetOuter()) : GEngine->GetCurrentPlayWorld();
#else
	return FWorldGetter::Get(GetOuter());
#endif
}

void UToroObject::BeginDestroy()
{
	SetTickableTickType(ETickableTickType::Never);
	Super::BeginDestroy();
}

void UToroObject::PostInitProperties()
{
	Super::PostInitProperties();
	if (!IsTemplate())
	{
		SetTickableTickType(ETickableTickType::Conditional);
		ReceiveInitialize();
	}
}

void UToroObject::Tick(float DeltaTime)
{
	ReceiveTick(DeltaTime);
}

bool UToroObject::IsTickable() const
{
	return bCanTick && !IsTemplate() && !HasAnyFlags(RF_BeginDestroyed);
}

bool UToroObject::IsTickableWhenPaused() const
{
	return bTickWhenPaused;
}

TStatId UToroObject::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(ToroObject, STATGROUP_Tickables);
}

#if WITH_EDITOR
bool UToroObject::IsTickableInEditor() const
{
	return bTickInEditor;
}
#endif
