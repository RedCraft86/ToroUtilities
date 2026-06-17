// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#include "ToroRuntimeSubsystem.h"
#include "ToroRuntime.h"

void UToroRuntimeSubsystem::AddGlobalMetadata(const FGameplayTag Key, const FInstancedStruct& Value)
{
	if (Key.IsValid())
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
	if (Key.IsValid())
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
	return Key.IsValid() && Metadata.Contains(Key);
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
	if (Key.IsValid())
	{
		return &Events.FindOrAdd(Key);
	}

	UE_LOG(LogToroRuntime, Warning, TEXT("Failed to get event because key '%s' is invalid"), *Key.ToString());
	return nullptr;
}
