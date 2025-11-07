// Copyright (C) RedCraft86. All Rights Reserved.

#include "Gameplay/WorldActions/Actions/EventActions.h"
#include "GlobalEvents/ToroEventManager.h"
#include "Engine/LevelScriptActor.h"
#include "Misc/OutputDeviceNull.h"

void FWTEvent_Remote::RunAction()
{
	if (EventName.IsNone()) return;
	const UWorld* World = GEngine->GetWorldFromContextObject(GetWorldContext(), EGetWorldErrorMode::LogAndReturnNull);
	if (ALevelScriptActor* LSA = World ? World->GetLevelScriptActor() : nullptr)
	{
		LSA->RemoteEvent(EventName);
	}
}

void FWTEvent_Actor::RunAction()
{
	if (EventName.IsNone()) return;

	FOutputDeviceNull Ar;
	for (TSoftObjectPtr<AActor>& Actor : Targets)
	{
		if (AActor* ActorPtr = Actor.LoadSynchronous())
		{
			ActorPtr->CallFunctionByNameWithArguments(*EventName.ToString(),
				Ar, nullptr, true);
		}
	}
}

void FWTEvent_Actor::OnBeginPlay()
{
	for (auto It = Targets.CreateIterator(); It; ++It)
	{
		if (It->IsNull()) It.RemoveCurrent();
	}
}

void FWTEvent_Global::RunAction()
{
	if (UToroEventManager* Manager = UToroEventManager::Get(GetWorldContext()))
	{
		UObject* Instigator = nullptr;
		UToroEventPayload* Payload = nullptr;
		if (bHasPayload)
		{
			Instigator = OptionalInvoker.LoadSynchronous();
			if (!Instigator)
			{
				Instigator = const_cast<UObject*>(GetWorldContext());
			}
			if (Payload = NewObject<UToroEventPayload>(Instigator); Payload)
			{
				Payload->Strings = PayloadStrings;
				Payload->Floats = PayloadFloats;
			}
		}

		Manager->InvokeGlobalEvent(EventID, Payload, Instigator);
	}
}
