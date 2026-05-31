// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "ToroRuntimeSubsystem.h"
#include "ToroGlobals.h"
#include "ToroRuntime.h"

void UToroRuntimeSubsystem::AddGlobalMetadata(const FGameplayTag Key, const FInstancedStruct& Value)
{
	if (ToroGameplayTags::Flag::IsValidTag(Key))
	{
		Metadata.Add(Key, Value);
	}
	else
	{
		UE_LOG(LogToroRuntime, Warning, TEXT("Failed to add metadata because key '%s' is invalid"), *Key.ToString());
	}
}

void UToroRuntimeSubsystem::RemoveGlobalMetadata(const FGameplayTag Key)
{
	if (ToroGameplayTags::Flag::IsValidTag(Key))
	{
		Metadata.Remove(Key);
	}
	else
	{
		UE_LOG(LogToroRuntime, Warning, TEXT("Failed to remove metadata because key '%s' is invalid"), *Key.ToString());
	}
}

bool UToroRuntimeSubsystem::HasGlobalMetadata(const FGameplayTag Key) const
{
	return ToroGameplayTags::Flag::IsValidTag(Key) && Metadata.Contains(Key);
}

const FInstancedStruct& UToroRuntimeSubsystem::GetGlobalMetadata(const FGameplayTag Key) const
{
	static const FInstancedStruct Empty = FInstancedStruct();
	return HasGlobalMetadata(Key) ? Metadata[Key] : Empty;
}

void UToroRuntimeSubsystem::BindGlobalEvent(const FGameplayTag Key, const FGlobalEventSingleDelegate& Event)
{
	if (!Event.IsBound()) return;
	if (FGlobalEventMultiDelegate* Delegate = GetEventDelegate(Key))
	{
		Delegate->Add(Event);
	}
}

void UToroRuntimeSubsystem::UnbindGlobalEvent(const FGameplayTag Key, const FGlobalEventSingleDelegate& Event)
{
	if (!Event.IsBound()) return;
	if (FGlobalEventMultiDelegate* Delegate = GetEventDelegate(Key))
	{
		Delegate->Remove(Event);
		if (!Delegate->IsBound()) // If empty, remove event
		{
			RemoveGlobalEvent(Key);
		}
	}
}

void UToroRuntimeSubsystem::InvokeGlobalEvent(const FGameplayTag Key, const FInstancedStruct& Payload, UObject* Instigator)
{
	if (const FGlobalEventMultiDelegate* Delegate = GetEventDelegate(Key))
	{
		Delegate->Broadcast(Payload, Instigator);
	}
}

void UToroRuntimeSubsystem::RemoveGlobalEvent(const FGameplayTag Key)
{
	if (FGlobalEventMultiDelegate* Delegate = GetEventDelegate(Key))
	{
		Delegate->Clear();
		Events.Remove(Key);
	}
}

FGlobalEventMultiDelegate* UToroRuntimeSubsystem::GetEventDelegate(const FGameplayTag& Key)
{
	if (ToroGameplayTags::Event::IsValidTag(Key))
	{
		return &Events.FindOrAdd(Key);
	}

	UE_LOG(LogToroRuntime, Warning, TEXT("Failed to get event because id '%s' is invalid"), *Key.ToString());
	return nullptr;
}
