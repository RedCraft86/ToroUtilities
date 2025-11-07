// Copyright (C) RedCraft86. All Rights Reserved.

#include "GlobalEvents/ToroEventManager.h"

namespace GlobalEvents
{
	DEFINE_GAMEPLAY_TAG(GlobalEvent)
}

void UToroEventManager::BindGlobalEvent(const FGameplayTag EventID, const FGlobalEventSingle& Event)
{
	if (GlobalEvents::IsValidTag(EventID) && Event.IsBound())
	{
		Events.FindOrAdd(EventID).Add(Event);
	}
}

void UToroEventManager::UnbindGlobalEvent(const FGameplayTag EventID, const FGlobalEventSingle& Event)
{
	if (!GlobalEvents::IsValidTag(EventID) || !Event.IsBound()) return;
	if (FGlobalEventMulti* MultiEvent = Events.Find(EventID))
	{
		MultiEvent->Remove(Event);
		if (!MultiEvent->IsBound())
		{
			RemoveGlobalEvent(EventID);
		}
	}
}

void UToroEventManager::InvokeGlobalEvent(const FGameplayTag EventID, UToroEventPayload* Payload, UObject* Instigator)
{
	if (!GlobalEvents::IsValidTag(EventID)) return;
	if (!Events.Contains(EventID)) return;
	if (Payload)
	{
		Payload->Invoker = Instigator;
		Payloads.Add(EventID, Payload);
	}
	else Payloads.Remove(EventID);
	Events.Find(EventID)->Broadcast(Payload);
}

void UToroEventManager::RemoveGlobalEvent(const FGameplayTag EventID)
{
	if (!GlobalEvents::IsValidTag(EventID)) return;
	if (FGlobalEventMulti* MultiEvent = Events.Find(EventID))
	{
		MultiEvent->Clear();
	}
	Events.Remove(EventID);
	Payloads.Remove(EventID);
}

UToroEventPayload* UToroEventManager::GetEventPayload(const FGameplayTag& EventID)
{
	return Payloads.Contains(EventID) ? *Payloads.Find(EventID) : nullptr;
}
